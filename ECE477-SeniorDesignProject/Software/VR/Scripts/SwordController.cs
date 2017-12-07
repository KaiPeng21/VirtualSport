using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SwordController : MonoBehaviour {

	AudioSource HitSwordAudioSource = null;

	private HapticManager haptic;

	[Tooltip("Enter the Magnitude of angle difference that triggers target update (high pass filter)")]
	public float AccelerameterSensitivity = 7;
	[Tooltip("Enter the target angle check period")]
	public float TargetAngleCheckPeriod = 0.05f;
	[Tooltip("Enter Haptic Feedback Latency")]
	public float HapticFeedbackLatency = 0.05f;
	[Tooltip("Enter Intensity threshold")]
	public float HighIntensityThreshold = 2.2f;
	public float MidIntensityThreshold = 1.5f;

	public static float PhysicalIntensity = 0;
	public static float accelerameterAngleX = 0;
	public static float accelerameterAngleZ = 0;

	public static Vector3 CurrentAngle = Vector3.zero;
	private static Vector3 targetAngle = Vector3.zero;
	private static Vector3 moveAnglePath = Vector3.zero;

	private enum AngleMovement{
		Forward,
		Backward,
		None
	}
	private AngleMovement angleMovement = AngleMovement.None;


	// Use this for initialization
	void Start () {
		HitSwordAudioSource = gameObject.AddComponent<AudioSource> ();
		HitSwordAudioSource.playOnAwake = false;
		HitSwordAudioSource.clip = Resources.Load <AudioClip> ("Hit");
		HitSwordAudioSource.spatialize = true;
		HitSwordAudioSource.spatialBlend = 1.0f;
		HitSwordAudioSource.dopplerLevel = 0.0f;
		HitSwordAudioSource.rolloffMode = AudioRolloffMode.Logarithmic;
		HitSwordAudioSource.maxDistance = 20f;

		haptic = GetComponentInParent<HapticManager> ();

		// smallest latency
		if (HapticFeedbackLatency < 0.05f) {
			HapticFeedbackLatency = 0.05f;
		}

		InvokeRepeating ("OnScanTargetData", 0, TargetAngleCheckPeriod);
	}

	void OnScanTargetData(){
		// get the accelerameter angles from raw data
		mapAccelXToAngleZ ();
		mapAccelYToAngleX ();
		Vector3 accelerameterAngles = new Vector3 (accelerameterAngleX, 0, accelerameterAngleZ);

		// High Pass Filter: update targetAngle only if the magnitude exceed the sensitivity threshold
		if (Vector3.Magnitude(accelerameterAngles - targetAngle) > AccelerameterSensitivity){
			targetAngle = accelerameterAngles;
			moveAnglePath = (targetAngle - CurrentAngle) / 10;
		}
	}

	// Update is called once per frame
	void Update () {

#if UNITY_EDITOR
		if (Input.GetKey(KeyCode.UpArrow)){
			transform.Rotate(new Vector3(0, 0, 1));
			angleMovement = AngleMovement.Forward;
		}
		else if (Input.GetKey (KeyCode.DownArrow)) {
			transform.Rotate (new Vector3 (0, 0, -1));
			angleMovement = AngleMovement.Backward;
		}

		if (Input.GetKey (KeyCode.LeftArrow)) {
			transform.Rotate (new Vector3 (-1, 0, 0));
		}
		else if (Input.GetKey (KeyCode.RightArrow)) {
			transform.Rotate (new Vector3 (1, 0, 0));
		}

#else
		if (Vector3.Magnitude(CurrentAngle - targetAngle) < AccelerameterSensitivity) {
			CurrentAngle = targetAngle;
		} else {
			CurrentAngle += moveAnglePath;
			if (targetAngle.z > CurrentAngle.z){
				angleMovement = AngleMovement.Forward;
			}else{
				angleMovement = AngleMovement.Backward;
			}
		}
		transform.localRotation = Quaternion.Euler (CurrentAngle);
#endif
	}

	private void mapAccelXToAngleZ(){
		if (BLEController.adxl_z < 0) {
			accelerameterAngleZ = BLEController.MapValue (BLEController.adxl_x, -270, 270, -90, 90);
		}
	}

	private void mapAccelYToAngleX(){
		if (BLEController.adxl_z < 0) {
			accelerameterAngleX = BLEController.MapValue (BLEController.adxl_y, -270, 270, -90, 90);
		}
	}


	// enter the collision event
	// if the object is interacible,
	//		play sound effect
	// 		determine haptic command and send it to the microcontroller
	void OnCollisionEnter(Collision collisionInfo){
		Debug.Log ("collisionInfo name: " + collisionInfo.collider.name);
		if (collisionInfo.gameObject.GetComponent<Interacible> () != null) {
			PlaySound ("Hit");
			StartCoroutine (HapticFeedbackStartCoroutine ());
		}
	}

	// collision stays
	// if the object is interacible,
	// 		log the physical intensity level
	//		
	void OnCollisionStay(Collision collisionInfo){
		if (collisionInfo.gameObject.GetComponent<Interacible> () != null) {
			PhysicalIntensity = Vector3.Magnitude (collisionInfo.impulse) + 1;
		}
	}

	// exit collision event
	void OnCollisionExit(Collision collisionInfo){
		if (collisionInfo.gameObject.GetComponent<Interacible> () != null) {
			StopSound ("Hit");
			StartCoroutine (HapticFeedbackEndCoroutine ());
		}
	}


	// When hit the target, determine the vibration and torque feedback
	// 		send the command to micro over bluetooth
	IEnumerator HapticFeedbackStartCoroutine(){
		yield return new WaitForSeconds (HapticFeedbackLatency);
		Debug.Log ("Physical: " + PhysicalIntensity);

		// set vibration level based on physical intensity
		if (PhysicalIntensity > HighIntensityThreshold) {
			haptic.SetVibrationLevel (HapticManager.VibrationType.HighVibration);
		} else if (PhysicalIntensity > MidIntensityThreshold) {
			haptic.SetVibrationLevel (HapticManager.VibrationType.MidVibration);
		} else {
			haptic.SetVibrationLevel (HapticManager.VibrationType.LowVibration);
		}

		if (angleMovement == AngleMovement.Forward) {
			haptic.SetTorqueFeedback (HapticManager.TorqueType.FowardTorque);
		} else if (angleMovement == AngleMovement.Backward) {
			haptic.SetTorqueFeedback (HapticManager.TorqueType.BackwardTorque);
		}

		haptic.SendHapticCommand ();
	}


	IEnumerator HapticFeedbackEndCoroutine(){
		yield return new WaitForSeconds (HapticFeedbackLatency);
		
	}

	private void PlaySound(string clipName){
		if (clipName == "Hit") {
			if (!HitSwordAudioSource.isPlaying) {
				HitSwordAudioSource.Play ();
			}
		}
	}

	private void StopSound(string clipName){
		if (clipName == "Hit") {
			HitSwordAudioSource.Stop ();
		}
	}
}
