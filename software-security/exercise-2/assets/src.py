import numpy as np
from scipy.optimize import differential_evolution
import math
import pwn

# --- Constants from the C Code ---
INPUT_SIZE = 15
HIDDEN1_SIZE = 8
HIDDEN2_SIZE = 6
OUTPUT_SIZE = 1
TARGET_PROBABILITY = 0.7331337420
EPSILON = 0.00001

XOR_KEYS = [
    0x42, 0x13, 0x37, 0x99, 0x21, 0x88, 0x45, 0x67,
    0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE
]

W1 = np.array([
    [0.523, -0.891, 0.234, 0.667, -0.445, 0.789, -0.123, 0.456],
    [-0.334, 0.778, -0.556, 0.223, 0.889, -0.667, 0.445, -0.221],
    [0.667, -0.234, 0.891, -0.445, 0.123, 0.556, -0.789, 0.334],
    [-0.778, 0.445, -0.223, 0.889, -0.556, 0.234, 0.667, -0.891],
    [0.123, -0.667, 0.889, -0.334, 0.556, -0.778, 0.445, 0.223],
    [-0.891, 0.556, -0.445, 0.778, -0.223, 0.334, -0.667, 0.889],
    [0.445, -0.123, 0.667, -0.889, 0.334, -0.556, 0.778, -0.234],
    [-0.556, 0.889, -0.334, 0.445, -0.778, 0.667, -0.223, 0.123],
    [0.778, -0.445, 0.556, -0.667, 0.223, -0.889, 0.334, -0.445],
    [-0.223, 0.667, -0.778, 0.334, -0.445, 0.556, -0.889, 0.778],
    [0.889, -0.334, 0.445, -0.556, 0.667, -0.223, 0.123, -0.667],
    [-0.445, 0.223, -0.889, 0.778, -0.334, 0.445, -0.556, 0.889],
    [0.334, -0.778, 0.223, -0.445, 0.889, -0.667, 0.556, -0.123],
    [-0.667, 0.889, -0.445, 0.223, -0.556, 0.778, -0.334, 0.667],
    [0.556, -0.223, 0.778, -0.889, 0.445, -0.334, 0.889, -0.556]
])

B1 = np.array([0.1, -0.2, 0.3, -0.15, 0.25, -0.35, 0.18, -0.28])

W2 = np.array([
    [0.712, -0.534, 0.823, -0.445, 0.667, -0.389],
    [-0.623, 0.889, -0.456, 0.734, -0.567, 0.445],
    [0.534, -0.712, 0.389, -0.823, 0.456, -0.667],
    [-0.889, 0.456, -0.734, 0.567, -0.623, 0.823],
    [0.445, -0.667, 0.823, -0.389, 0.712, -0.534],
    [-0.734, 0.623, -0.567, 0.889, -0.456, 0.389],
    [0.667, -0.389, 0.534, -0.712, 0.623, -0.823],
    [-0.456, 0.823, -0.667, 0.445, -0.889, 0.734]
])

B2 = np.array([0.05, -0.12, 0.18, -0.08, 0.22, -0.16])

W3 = np.array([
    [0.923], [-0.812], [0.745], [-0.634], [0.856], [-0.723]
])

B3 = np.array([0.42])

# --- Logic Implementation ---

def xor_activate(x, key):
    # C casts to long, XORs, then casts back
    long_val = int(x * 1000000) 
    long_val ^= key
    return float(long_val) / 1000000.0

def tanh_activate(x):
    return math.tanh(x)

def cos_activate(x):
    return math.cos(x)

def sinh_activate(x):
    return math.sinh(x / 10.0)

def sigmoid(x):
    # Prevent overflow in exp
    if x < -700: return 0.0
    if x > 700: return 1.0
    return 1.0 / (1.0 + math.exp(-x))

def forward_pass(inputs):
    # Layer 1 Calculation
    hidden1 = np.zeros(HIDDEN1_SIZE)
    
    # We must calculate the "activated" input vector first
    # This matches the C loop logic: inputs[i] is activated then multiplied by W1[i][j]
    activated_inputs = np.zeros(INPUT_SIZE)
    for i in range(INPUT_SIZE):
        val = inputs[i]
        switch = i % 4
        if switch == 0:
            activated_inputs[i] = xor_activate(val, XOR_KEYS[i])
        elif switch == 1:
            activated_inputs[i] = tanh_activate(val)
        elif switch == 2:
            activated_inputs[i] = cos_activate(val)
        elif switch == 3:
            activated_inputs[i] = sinh_activate(val)
            
    # Matrix Multiplication for Layer 1
    # hidden1[j] = sum(activated_inputs[i] * W1[i][j]) + B1[j]
    hidden1 = np.dot(activated_inputs, W1) + B1
    hidden1 = np.tanh(hidden1) # Layer 1 activation is tanh
    
    # Layer 2 Calculation
    # hidden2[j] = sum(hidden1[i] * W2[i][j]) + B2[j]
    hidden2 = np.dot(hidden1, W2) + B2
    hidden2 = np.tanh(hidden2) # Layer 2 activation is tanh
    
    # Output Calculation
    output_val = np.dot(hidden2, W3) + B3
    result = sigmoid(output_val[0])
    
    return result

# --- Optimization ---

def objective_function(x):
    prob = forward_pass(x)
    # We want to minimize the difference between output and target
    return abs(prob - TARGET_PROBABILITY)

print("Starting solver... This might take a few seconds.")

# We set bounds roughly based on realistic human stats, 
# but wide enough to allow the AI to find a fit.
# Q1 Height: 100-250, Q2 Weight: 30-200, etc.
# For simplicity and to ensure a solution, we use generic bounds.
bounds = [(-100, 300)] * INPUT_SIZE

result = differential_evolution(
    objective_function, 
    bounds, 
    strategy='best1bin', 
    maxiter=1000, 
    popsize=20, 
    tol=1e-7,
    seed=42 # For reproducibility
)

print(f"\nSuccess: {result.success}")
print(f"Final Error: {result.fun}")
print("-" * 30)
print("INPUT VALUES TO ENTER:")
for i, val in enumerate(result.x):
    print(f"Q{i+1}: {val:.6f}")

print("-" * 30)
final_prob = forward_pass(result.x)
print(f"Calculated Probability: {final_prob:.10f}")
print(f"Target Probability:     {TARGET_PROBABILITY:.10f}")

# SENDING DATA THROUGH TCP
HOST = "floating.chals.nitectf25.live"
PORT = 1337

r = pwn.remote(HOST, PORT)
print("Sending calculated inputs")

for (i,val) in enumerate(result.x):
    # Receive text until the prompt (waiting for the question)
    # The prompt ends with "? " based on the C code.
    r.recvuntil(b'?')

    # format the number as a string
    payload = f"{val:f}"
    print(f"Sending Q{i+1}: {payload}")
    r.sendline(payload.encode())

print("\n--- SERVER RESPONSE ---")
r.interactive()
