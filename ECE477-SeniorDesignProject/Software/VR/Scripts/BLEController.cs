using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BLEController : MonoBehaviour {

	// Bluetooth Device Parameters
	[Tooltip("Enter BLE Device Name")]
	public string DeviceName1 = "HMSoft";
	[Tooltip("Enter an alternative BLE Device Name")]
	public string DeviceName2 = "SH-HC-08";
	[Tooltip("Enter service UUID")]
	public string ServiceUUID = "FFE0";
	[Tooltip("Enter characteristic UUID")]
	public string SerialCharacteristic = "FFE1";

	// Bluetooth Controller States
	enum States
	{
		None,
		Scan,
		ScanRSSI,
		Connect,
		Subscribe,
		Unsubscribe,
		Disconnect,
	}

	private bool _transmit_bug = false;

	// Bluetooth Controller Parameters
	private bool _connected = false;
	private float _timeout = 0f;
	private States _state = States.None;
	private string _deviceAddress;
	private bool _foundSerialID = false;
	private byte[] _dataBytes = null;
	private bool _rssiOnly = false;
	private int _rssi = 0;

	// Data from microcontroller
	public static short adxl_x { get; private set; }
	public static short adxl_y { get; private set; }
	public static short adxl_z { get; private set; }

	public static bool pushSEL;
	public static bool pushINC;
	public static bool pushDEC;
	public static bool pushBEF;
	public static bool pushNXT;

	public static bool[] prevSEL = new bool[6];
	public static bool[] prevINC = new bool[6];
	public static bool[] prevDEC = new bool[6];
	public static bool[] prevBEF = new bool[6];
	public static bool[] prevNXT = new bool[6];

	// BLE command
	public static byte command;

	void BleReset ()
	{

		// Reset the Bluetooth Controller Parameters
		_connected = false;
		_timeout = 0f;
		_state = States.None;
		_deviceAddress = null;
		_foundSerialID = false;
		_dataBytes = null;
		_rssi = 0;
	}

	// Change the Bluetooth controller state
	void BleSetState (States newState, float timeout)
	{
		_state = newState;
		_timeout = timeout;
	}

	void BleStartProcess ()
	{
		BleReset ();
		BluetoothLEHardwareInterface.Initialize (true, false, () => {

			BleSetState (States.Scan, 0.1f);

		}, (error) => {

			BluetoothLEHardwareInterface.Log ("Error during initialize: " + error);
		});
	}

	string BleFullUUID (string uuid)
	{
		return "0000" + uuid + "-0000-1000-8000-00805f9b34fb";
	}

	bool BleIsEqual(string uuid1, string uuid2)
	{
		if (uuid1.Length == 4)
			uuid1 = BleFullUUID (uuid1);
		if (uuid2.Length == 4)
			uuid2 = BleFullUUID (uuid2);

		return (uuid1.ToUpper().CompareTo(uuid2.ToUpper()) == 0);
	}

	public void BleSendByte (byte value)
	{
		byte[] data = new byte[] { value };
		bool writeWithResponse = false;
		BluetoothLEHardwareInterface.WriteCharacteristic (_deviceAddress, ServiceUUID, SerialCharacteristic, data, data.Length, writeWithResponse, (characteristicUUID) => {

			BluetoothLEHardwareInterface.Log ("Write Succeeded");
		});
		command = value;
		Debug.Log ("Bluetooth Send: " + value);
	}

	string BleReadData()
	{
		return System.Text.Encoding.ASCII.GetString(_dataBytes);
	}

	// Parse raw data received from the BLE
	/*
	void BleParseInputData(string rawData)
	{
		try
		{
			string[] vec = rawData.Split(',');
			adxl_x = int.Parse(vec[0]);
			adxl_y = int.Parse(vec[1]);
			adxl_z = int.Parse(vec[2]);

			_transmit_bug = false;

		}catch (System.Exception e ){
			_transmit_bug = true;
			Debug.Log (e);
		}
	}
	*/

	void BleCheckPushbuttons()
	{
		try{
			pushSEL = (_dataBytes[9] & 16) != 0;
			pushINC = (_dataBytes[9] & 8) != 0;
			pushDEC = (_dataBytes[9] & 4) != 0;
			pushBEF = (_dataBytes[9] & 2) != 0;
			pushNXT = (_dataBytes[9] & 1) != 0;

		} catch (System.Exception e){
			Debug.Log (e);
		}

		if (prevSEL[0] && prevSEL[1] && !prevSEL[2] && !prevSEL[3] && !prevSEL[4] && !prevSEL[5] && !pushSEL) {
			BroadcastMessage ("OnNextScene");
		}
		if (prevINC[0] && prevINC[1] && !prevINC[2] && !prevINC[3] && !prevINC[4] && !prevINC[5] &&  !pushINC) {

		}
		if (prevDEC[0] && prevDEC[1] && !prevDEC[2] && !prevDEC[3] && !prevDEC[4] && !prevDEC[5] &&  !pushDEC) {

		}
		if (prevBEF[0] && prevBEF[1] && !prevBEF[2] && !prevBEF[3] && !prevBEF[4] && !prevSEL[5] &&  !pushBEF) {
			
		}
		if (prevNXT[0] && prevNXT[1] && !prevNXT[2] && !prevNXT[3] && !prevNXT[4] && !prevNXT[5] &&  !pushNXT) {
			
		}

		int len = prevSEL.Length;
		for (int i = 0; i < len-1; i++) {
			prevSEL[i] = prevSEL[i+1];
			prevINC[i] = prevINC[i+1];
			prevDEC[i] = prevDEC[i+1];
			prevBEF[i] = prevBEF[i+1];
			prevNXT[i] = prevNXT[i+1];
		}
		prevSEL[len-1] = pushSEL;
		prevINC[len-1] = pushINC;
		prevDEC[len-1] = pushDEC;
		prevBEF[len-1] = pushBEF;
		prevNXT[len-1] = pushNXT;
	}

	void BleParseInputData()
	{
		try{
			adxl_x = System.BitConverter.ToInt16(_dataBytes, 0);
			adxl_y = System.BitConverter.ToInt16(_dataBytes, 3);
			adxl_z = System.BitConverter.ToInt16(_dataBytes, 6);

		} catch (System.Exception e){
			Debug.Log (e);
		}
	}

	// scale value
	static public float MapValue(float data_in, float min_in, float max_in, float min_out, float max_out)
	{
		return (data_in - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
	}

	void Awake ()
	{
		DontDestroyOnLoad (transform.gameObject);
	}

	// Use this for initialization
	void Start ()
	{
		BleStartProcess ();
		StartCoroutine (BlePingUserInput());
	}

	// Update is called once per frame
	void Update ()
	{
		if (_timeout > 0f)
		{
			_timeout -= Time.deltaTime;
			if (_timeout <= 0f)
			{
				_timeout = 0f;

				switch (_state)
				{
				case States.None:
					break;

				case States.Scan:
					BluetoothLEHardwareInterface.ScanForPeripheralsWithServices (null, (address, name) => {

						// if your device does not advertise the rssi and manufacturer specific data
						// then you must use this callback because the next callback only gets called
						// if you have manufacturer specific data

						if (!_rssiOnly)
						{
							if (name.Contains (DeviceName1) || name.Contains(DeviceName2))
							{
								BluetoothLEHardwareInterface.StopScan ();

								// found a device with the name we want
								// this example does not deal with finding more than one
								_deviceAddress = address;
								BleSetState (States.Connect, 0.5f);
							}
						}

					}, (address, name, rssi, bytes) => {

						// use this one if the device responses with manufacturer specific data and the rssi

						if (name.Contains (DeviceName1) || name.Contains(DeviceName2))
						{
							if (_rssiOnly)
							{
								_rssi = rssi;
							}
							else
							{
								BluetoothLEHardwareInterface.StopScan ();

								// found a device with the name we want
								// this example does not deal with finding more than one
								_deviceAddress = address;
								BleSetState (States.Connect, 0.5f);
							}
						}

					}, _rssiOnly); // this last setting allows BLE to send RSSI without having manufacturer data

					if (_rssiOnly)
						BleSetState (States.ScanRSSI, 0.5f);
					break;

				case States.ScanRSSI:
					break;

				case States.Connect:
					// set these flags
					_foundSerialID = false;

					// note that the first parameter is the address, not the name. I have not fixed this because
					// of backwards compatiblity.
					// also note that I am note using the first 2 callbacks. If you are not looking for specific characteristics you can use one of
					// the first 2, but keep in mind that the device will enumerate everything and so you will want to have a timeout
					// large enough that it will be finished enumerating before you try to subscribe or do any other operations.
					BluetoothLEHardwareInterface.ConnectToPeripheral (_deviceAddress, null, null, (address, serviceUUID, characteristicUUID) => {

						if (BleIsEqual (serviceUUID, ServiceUUID))
						{
							_foundSerialID = _foundSerialID || BleIsEqual (characteristicUUID, SerialCharacteristic);

							// if we have found both characteristics that we are waiting for
							// set the state. make sure there is enough timeout that if the
							// device is still enumerating other characteristics it finishes
							// before we try to subscribe
							if (_foundSerialID)
							{
								_connected = true;
								BleSetState (States.Subscribe, 2f);
							}
						}
					});
					break;

				case States.Subscribe:
					BluetoothLEHardwareInterface.SubscribeCharacteristicWithDeviceAddress (_deviceAddress, ServiceUUID, SerialCharacteristic, null, (address, characteristicUUID, bytes) => {

						_state = States.None;

						// data received from the device
						_dataBytes = bytes;



					});
					break;

				case States.Unsubscribe:
					BluetoothLEHardwareInterface.UnSubscribeCharacteristic (_deviceAddress, ServiceUUID, SerialCharacteristic, null);
					BleSetState (States.Disconnect, 4f);
					break;

				case States.Disconnect:
					if (_connected)
					{
						BluetoothLEHardwareInterface.DisconnectPeripheral (_deviceAddress, (address) => {
							BluetoothLEHardwareInterface.DeInitialize (() => {

								_connected = false;
								_state = States.None;
							});
						});
					}
					else
					{
						BluetoothLEHardwareInterface.DeInitialize (() => {

							_state = States.None;
						});
					}
					break;
				}
			}
		}
	}



	IEnumerator BlePingUserInput(){
		while (true) {
			yield return new WaitForSeconds (0.1f);
			if (_connected) {
				BleCheckPushbuttons ();
				BleParseInputData ();
			}
		}
	}

	void OnGUI()
	{
		GUI.skin.label.alignment = TextAnchor.MiddleCenter;
		GUI.skin.button.alignment = TextAnchor.MiddleCenter;
		GUI.skin.label.fontSize = 40;
		GUI.skin.button.fontSize = 15;

		if (_connected) {
			

#if !UNITY_EDITOR
			DebugText.SetText ("adxl_x: " + adxl_x + " adxl_y" + adxl_y + " adxl_z" + adxl_z + "pushbuttons: " + pushSEL + " " + pushINC + " " + pushDEC + " " + pushBEF + " " + pushNXT);
#endif

			_state = States.None;

			// debugging pushbuttons on the screen
			if (GUI.Button (new Rect (10, 610, 90, 100), "VH F")) {
				BleSendByte ((byte)(HapticManager.HCmdVibHigh << 4 | HapticManager.HCmdTorqueForward));
			} else if (GUI.Button (new Rect (110, 610, 90, 100), "VM F")) {
				BleSendByte ((byte)(HapticManager.HCmdVibMid << 4 | HapticManager.HCmdTorqueForward));
			} else if (GUI.Button (new Rect (210, 610, 90, 100), "VL F")) {
				BleSendByte ((byte)(HapticManager.HCmdVibLow << 4 | HapticManager.HCmdTorqueForward));
			} else if (GUI.Button (new Rect (310, 610, 90, 100), "VH B")) {
				BleSendByte ((byte)(HapticManager.HCmdVibHigh << 4 | HapticManager.HCmdTorqueBackward));
			} else if (GUI.Button (new Rect (410, 610, 90, 100), "VM B")) {
				BleSendByte ((byte)(HapticManager.HCmdVibMid << 4 | HapticManager.HCmdTorqueBackward));
			} else if (GUI.Button (new Rect (510, 610, 90, 100), "VL B")) {
				BleSendByte ((byte)(HapticManager.HCmdVibLow << 4 | HapticManager.HCmdTorqueBackward));
			} else {
				_state = States.Subscribe;
			}

		} else {
			GUI.skin.label.fontSize = 40;
			GUI.skin.label.alignment = TextAnchor.MiddleCenter;
#if !UNITY_EDITOR
			GUI.Label (new Rect (10, 10, Screen.width - 10, Screen.height - 10), "Haptic Device Not Found");
#endif
		}
	}
}
