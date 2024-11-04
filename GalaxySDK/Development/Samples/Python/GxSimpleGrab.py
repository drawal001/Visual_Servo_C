# version:1.0.2406.9261

import gxipy as gx
from gxipy.gxidef import *
import threading

thread_state = True


def capture_thread(cam):
    cam.stream_on()
    while thread_state:
        try:
            image = cam.data_stream[0].dq_buf(100)
            if image.frame_data.status == GxFrameStatusList.SUCCESS:
                print(f"<Successful acquisition: Width: {image.frame_data.width}, " +
                      f"Height: {image.frame_data.height}, FrameID: {image.frame_data.frame_id}>")
            else:
                print("<Abnormal Acquisition>")
            cam.data_stream[0].q_buf(image)
        except Exception as ex:
            print(f"error: {str(ex)}")

    cam.stream_off()


def main():
    # create a device manager
    device_manager = gx.DeviceManager()
    dev_num, dev_info_list = device_manager.update_all_device_list()
    if dev_num == 0:
        print("Number of enumerated devices is 0")
        return

    # open the first device
    cam = device_manager.open_device_by_index(1)
    remote_device_feature = cam.get_remote_device_feature_control()

    # Restore default parameter group
    remote_device_feature.get_enum_feature("UserSetSelector").set("Default")
    remote_device_feature.get_command_feature("UserSetLoad").send_command()

    print("***********************************************")
    print(f"<Vendor Name:   {dev_info_list[0]['vendor_name']}>")
    print(f"<Model Name:    {dev_info_list[0]['model_name']}>")
    print("***********************************************")
    print("Press [a] or [A] and then press [Enter] to start acquisition")
    print("Press [x] or [X] and then press [Enter] to Exit the Program")

    wait_start = True
    while wait_start:
        user_input = input()
        if user_input == 'A' or user_input == 'a':
            wait_start = False
        elif user_input == 'X' or user_input == 'x':
            print("<App exit!>")
            return

    capture_thread(cam)
    cam.close_device()


if __name__ == "__main__":
    main()
