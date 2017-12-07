using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoxManager : MonoBehaviour {

	Dictionary<string, Vector3> originalPositions;
	Dictionary<string, Quaternion> originalRotations;

	// Use this for initialization
	void Start () {
		originalPositions = new Dictionary<string, Vector3> ();
		originalRotations = new Dictionary<string, Quaternion> ();
		foreach (Transform child in transform) {
			// save original positions
			originalPositions.Add (child.name, child.position);
			originalRotations.Add (child.name, child.rotation);
		}
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown(KeyCode.R)){
			OnResetBoxes();
		}
	}

	public void OnResetBoxes () {
		// Destroy Rigid Bodies
		foreach (Transform child in transform) {
			Rigidbody rigidBody = child.GetComponent<Rigidbody> ();
			rigidBody.isKinematic = true;
		}

		foreach (Transform child in transform) {
			child.position = originalPositions [child.name];
			child.rotation = originalRotations [child.name];
		}

		foreach (Transform child in transform) {
			Rigidbody rigidBody = child.GetComponent<Rigidbody> ();
			rigidBody.isKinematic = false;
		}
	}
}
