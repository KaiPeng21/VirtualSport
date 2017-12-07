using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RopeMotion : MonoBehaviour {

	// Use this for initialization
	void Start () {
		GetComponent<CharacterJoint> ().connectedBody = transform.parent.GetComponent<Rigidbody> ();
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
