using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WoodDisorderTrigger : MonoBehaviour {

	private Vector3 originalPosition;
	public float TolerableDisorderRange = 10.0f;
	public float CurrentDisorder { get; private set;}

	// Use this for initialization
	void Start () {
		originalPosition = transform.localPosition;
	}
	
	// Update is called once per frame
	void Update () {
		CurrentDisorder = Vector3.Magnitude (transform.localPosition - originalPosition);
		if (CurrentDisorder > TolerableDisorderRange) {
			SendMessageUpwards ("OnResetRope");
		}
	}
}
