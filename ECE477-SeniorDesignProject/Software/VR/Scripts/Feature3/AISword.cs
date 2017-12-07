using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AISword : MonoBehaviour {

	AudioSource DropSwordAudioSource = null;

	public GameObject Hand;
	public GameObject Sword;
	public GameObject Floor;
	public int Life = 3;

	private Vector3 originalPosition;
	private Quaternion originalRotation;

	private bool isResetting = false;

	public static bool IsHoldingSword = true;

	// Use this for initialization
	void Start () {
		originalPosition = transform.position;
		originalRotation = transform.rotation;

		DropSwordAudioSource = gameObject.AddComponent<AudioSource> ();
		DropSwordAudioSource.playOnAwake = false;
		DropSwordAudioSource.clip = Resources.Load <AudioClip> ("DropSword");
		DropSwordAudioSource.spatialize = true;
		DropSwordAudioSource.spatialBlend = 1.0f;
		DropSwordAudioSource.dopplerLevel = 0.0f;
		DropSwordAudioSource.rolloffMode = AudioRolloffMode.Logarithmic;
		DropSwordAudioSource.maxDistance = 20f;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void OnResetAI(){
		transform.position = originalPosition;
		transform.rotation = originalRotation;
		FixedJoint fixedJoint = gameObject.AddComponent<FixedJoint> ();
		fixedJoint.connectedBody = Hand.GetComponent<Rigidbody> ();
		GetComponent<Rigidbody> ().constraints = RigidbodyConstraints.FreezePosition;
		isResetting = false;
		IsHoldingSword = true;
		Life = 3;
	}

	public void OnDrop(){
		Destroy (gameObject.GetComponent<FixedJoint> ());
		GetComponent<Rigidbody> ().constraints = RigidbodyConstraints.None;
		IsHoldingSword = false;
		Debug.Log ("Drop");
	}

	void OnCollisionEnter(Collision collisionInfo){
		if (collisionInfo.collider.name == Sword.name) {
			Life--;
			if (Life == 0) {
				OnDrop ();
			}
		} else if (collisionInfo.collider.name == Floor.name) {
			if (!isResetting) {
				isResetting = true;
				StartCoroutine (ResetAIOrderRoutine());
			}
		}
	}

	private IEnumerator ResetAIOrderRoutine(){
		PlaySound ("DropSword");
		yield return new WaitForSeconds (3.0f);
		SendMessageUpwards ("OnResetAIOrder");
	}

	public void PlaySound(string clipName){
		if (clipName == "DropSword") {
			if (!DropSwordAudioSource.isPlaying) {
				DropSwordAudioSource.Play ();
			}
		}
	}

}
