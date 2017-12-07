using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AIRotationMovement : MonoBehaviour {

	public GameObject Target;

	public bool IsAnimating { get; private set; }
	public bool IsMoveToTarget { get; private set; }
	public bool IsMoveAwayTarget { get; private set; }

	private Vector3 originalPosition;
	private Quaternion originalRotation;

	private Vector3 awayAngle;
	private Vector3 toAngle;

	public int AnimationSteps = 50;
	private int animationCount = 0;

	// Use this for initialization
	void Start () {
		originalPosition = transform.position;
		originalRotation = transform.rotation;

		awayAngle = transform.localRotation.eulerAngles;
		toAngle = Target.transform.localRotation.eulerAngles;

		InvokeRepeating ("OnMovement", 0, 0.01f);
	}

	void OnResetAI(){
		transform.position = originalPosition;
		transform.rotation = originalRotation;
	}

	// Update is called once per frame
	void Update () {

	}

	void OnMovement(){
		if (IsAnimating) {
			if (IsMoveToTarget) {
				Vector3 path = (toAngle - awayAngle) / AnimationSteps;
				transform.localRotation = Quaternion.Euler (transform.localRotation.eulerAngles + path);

				if (animationCount >= AnimationSteps) {
					animationCount = AnimationSteps;
					MoveAwayTarget ();
				} else {
					animationCount++;
				}
			} else if (IsMoveAwayTarget) {
				Vector3 path = (awayAngle - toAngle) / AnimationSteps;
				transform.localRotation = Quaternion.Euler (transform.localRotation.eulerAngles + path);

				if (animationCount <= 0) {
					animationCount = 0;
					StopMovement ();
				} else {
					animationCount--;
				}
			}
		}
	}

	private void MoveAwayTarget(){
		IsMoveAwayTarget = true;
		IsMoveToTarget = false;
	}

	private void MoveToTarget(){
		IsMoveAwayTarget = false;
		IsMoveToTarget = true;
	}

	private void StopMovement(){
		IsMoveAwayTarget = false;
		IsMoveToTarget = false;
		IsAnimating = false;
	}

	public void OnAnimationTo(){
		if (!IsAnimating) {
			IsAnimating = true;
			MoveToTarget ();
		}
	}

	public void OnAnimationAway(){
		if (!IsAnimating) {
			IsAnimating = true;
			MoveAwayTarget ();
		}
	}

	public void OnAnimationPause(){
		if (IsAnimating) {
			IsAnimating = false;
		}
	}

	public void OnAnimationResume(){
		if (!IsAnimating) {
			IsAnimating = true;
		}
	}

	public void OnAnimationToggle(){
		IsAnimating = !IsAnimating;
	}
}
