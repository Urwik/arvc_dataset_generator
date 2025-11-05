#!/usr/bin/env python3
import psutil
import time
import roslaunch
import rospy
import os
import subprocess

def run_roscore():
    roscore_process = subprocess.Popen(['roscore'])
    return roscore_process


def get_processes_by_name(proc_names_):
    process_list = []
    for process in psutil.process_iter():
        if process.name() in proc_names_:
            process_list.append(process)
    
    print(f'Processes: {process_list}')
    return process_list

def check_processes_live(proc_list_):
    while True:
        for proc in proc_list_: 
            if proc.is_running():
                pass
            else:
                print(f'A process has died. Killing and restarting launch')
                return False
    
        time.sleep(10)

def kill_processes(proc_list_):
    for proc in proc_list_:
        try:
            proc.kill()
        except:
            pass
    print(f'All related process killed, sleeping for 10 seconds...')
    time.sleep(10)

def run_launch(launch_path_):
    print(f'Re-running launchfile')
    rospy.init_node('dataset_generator', anonymous=True)
    uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
    roslaunch.configure_logging(uuid)
    launch = roslaunch.parent.ROSLaunchParent(uuid, [launch_path_])
    launch.start()
    # rospy.loginfo("started")

    time.sleep(5)

def get_num_files_in_directory(directory_path):
    return len([f for f in os.listdir(directory_path) if os.path.isfile(os.path.join(directory_path, f))])


if __name__ == '__main__':
    p_names = ["gzserver", "gzclient"]
    launch_path = "/home/arvc/workspaces/arvc_ws/src/arvc_dataset_generator/launch/test_data_generator.launch" # Modify
    pcd_path = "/home/arvc/datasets/arvc_truss/test/crossed/pcd" # Modify

    roscore = run_roscore()
    run_launch(launch_path_=launch_path)


    while get_num_files_in_directory(pcd_path) < 1000:
        proc_list = get_processes_by_name(p_names)
        check_processes_live(proc_list_=proc_list)
        kill_processes(proc_list_=proc_list)
        run_launch(launch_path_=launch_path)
        
    kill_processes(proc_list_=proc_list)
    roscore.kill()
    print(f'All processes killed, roscore killed. Exiting...')
    # os.system('poweroff')
    
    
