import psutil
import time

p_name = ["gzserver", "gzclient"]
gz_processes = []

for process in psutil.process_iter():
  if process.name() in p_name:
    gz_processes.append(process)    




def checkRunningProcesses():
  while True:
    for process in gz_processes:
      if process.is_running():
        pass
      else:
        return False
    
    time.sleep(10)
    
