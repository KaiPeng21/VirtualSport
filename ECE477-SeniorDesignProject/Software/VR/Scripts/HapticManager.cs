using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;

public class HapticManager : MonoBehaviour {

	private BLEController bluetooth;

	// command to be send to microcontroller
	public static byte HCmdTorqueForward = (byte) 6;
	public static byte HCmdTorqueBackward = (byte) 7;
	public static byte HCmdVibHigh = (byte) 5;
	public static byte HCmdVibMid = (byte) 6;
	public static byte HCmdVibLow = (byte)7;

	public byte HapticCommand { get; private set; }

	// intensity of the vibraiton motor
	public enum VibrationType{
		NoVibration = 0,
		LowVibration = 1,
		MidVibration = 2,
		HighVibration = 3,
	} 

	// torque direction
	public enum TorqueType{
		NoTorque = 0,
		FowardTorque = 1,
		BackwardTorque = 2,
	}

	void Start(){
		bluetooth = GetComponent<BLEController> ();
		HapticCommand = (byte)0;
	}

	// vibration level is higher 4 bit of Haptic Command
	public void SetVibrationLevel(VibrationType vibrationType){
		HapticCommand = (byte) (HapticCommand & (byte)15); // 0x0F, clear the upper 4 bit

		// embed the upper 4 bit with new vibration level
		if (vibrationType == VibrationType.HighVibration) {
			HapticCommand = (byte)(HapticCommand | (byte)(HCmdVibHigh << 4));
		} else if (vibrationType == VibrationType.MidVibration) {
			HapticCommand = (byte)(HapticCommand | (byte)(HCmdVibMid << 4));
		} else if (vibrationType == VibrationType.LowVibration) {
			HapticCommand = (byte)(HapticCommand | (byte)(HCmdVibLow << 4));
		}
	}

	// torque direction is lower 4 bit of Haptic Command 
	public void SetTorqueFeedback(TorqueType torqueType){
		HapticCommand = (byte)(HapticCommand & (byte)240); // 0xF0, clear the lower 4 bit

		// embed the lower 4 bit with new torque direction
		if (torqueType == TorqueType.FowardTorque) {
			HapticCommand = (byte)(HapticCommand | HCmdTorqueForward);
		} else if (torqueType == TorqueType.BackwardTorque) {
			HapticCommand = (byte)(HapticCommand | HCmdTorqueBackward);
		} 
	}

	// request the bluetooth controller to send haptic command to the microcontroller
	public void SendHapticCommand(){
		bluetooth.BleSendByte (HapticCommand);
	}

}
