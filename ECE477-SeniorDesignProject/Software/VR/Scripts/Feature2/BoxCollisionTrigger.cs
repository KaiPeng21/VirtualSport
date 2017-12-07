using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoxCollisionTrigger : MonoBehaviour {

	[Tooltip("Drag the floor GameObject here")]
	public GameObject Floor;
	public GameObject Sword;

	AudioSource HitGroundAudioSource = null;



	public static int ResetTriggerCollisionNumber = 2;
	private static int resetTriggerCollisionCount;

	// Use this for initialization
	void Start () {
		resetTriggerCollisionCount = 0;

		HitGroundAudioSource = gameObject.AddComponent<AudioSource>();
		HitGroundAudioSource.playOnAwake = false;
		HitGroundAudioSource.clip = Resources.Load <AudioClip> ("DropWood");
		HitGroundAudioSource.spatialize = true;
		HitGroundAudioSource.spatialBlend = 1.0f;
		HitGroundAudioSource.dopplerLevel = 0.0f;
		HitGroundAudioSource.rolloffMode = AudioRolloffMode.Logarithmic;
		HitGroundAudioSource.maxDistance = 20f;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void OnCollisionEnter(Collision collisionInfo){
		if (collisionInfo.collider.name == Floor.name) {
			PlaySound ("DropWood");
			resetTriggerCollisionCount++;
			if (resetTriggerCollisionCount == ResetTriggerCollisionNumber) {
				StartCoroutine (Order ());
			}
		}
	}

	private IEnumerator Order(){
		yield return new WaitForSeconds(3.0f);
		SendMessageUpwards("OnResetBoxes");
		resetTriggerCollisionCount = 0;
	}

	private void PlaySound(string clipName){
		if (clipName == "DropWood") {
			if (!HitGroundAudioSource.isPlaying) {
				HitGroundAudioSource.Play ();
			}
		}

	}
}
