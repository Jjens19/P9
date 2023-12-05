import subprocess

# Path to your ns-3 script file
ns3_script = "scratch/test2.cc"

# Run the ns-3 simulation as a subprocess
process = subprocess.Popen(["./ns3", "run", ns3_script], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

# Optionally, you can read the simulation output or wait for it to finish
output, error = process.communicate()

print("Simulation Output:")
print(output,error)
