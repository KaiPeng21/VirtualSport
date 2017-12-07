using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RopeManager : MonoBehaviour {

	public GameObject RopeCollection;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKey(KeyCode.R)){
			OnResetRope();
		}
	}

	public void OnResetRope(){
		foreach (Transform child in transform) {
			Destroy (child.gameObject);
		}
		Instantiate (RopeCollection, transform);
	}
}
