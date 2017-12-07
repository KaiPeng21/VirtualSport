using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AIController : MonoBehaviour {


	public float AnimationPeriod = 3.0f;

	// Use this for initialization
	void Start () {
		InvokeRepeating("OnCheckAnimation", 0, AnimationPeriod);

	}

	private void OnCheckAnimation(){
		if (AISword.IsHoldingSword) {
			BroadcastMessage ("OnAnimationTo");
		}
	}

	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.M)) {
			BroadcastMessage ("OnAnimationTo");
		}
	}

	public void OnResetAIOrder(){
		BroadcastMessage ("OnResetAI");
	}

	public void OnReset(){
		OnResetAIOrder ();
	}


}
