import subprocess
import time

# Path to your ns-3 script file
ns3_script = "scratch/test6.cc"

# Run the ns-3 simulation as a subprocess
process = subprocess.Popen(["./ns3", "run", ns3_script], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

while "Simulation" not in process.stdout.readline(): 
    time.sleep(0.02)



cwnd = process.stdout.readline()
while "Done" not in cwnd: 
	time.sleep(0.002) 
	
	if cwnd != "":
		print(cwnd.split('\n')[0])
		process.stdin.write(cwnd.split('\n')[0]+"\n")
		process.stdin.flush()
		
	cwnd = process.stdout.readline()


