import psutil
import time
import roslaunch
import rospy

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


if __name__ == '__main__':
    p_names = ["gzserver", "gzclient"]
    p_names = ["usb_cam_node", "roslaunch", "image_view"]

    launch_path = "/opt/ros/noetic/share/usb_cam/launch/usb_cam-test.launch"
    run_launch(launch_path_=launch_path)
    while True:
        proc_list = get_processes_by_name(p_names)
        check_processes_live(proc_list_=proc_list)
        kill_processes(proc_list_=proc_list)
        run_launch(launch_path_=launch_path)

