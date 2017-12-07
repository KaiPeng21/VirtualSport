using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FeatureManager : MonoBehaviour {

	public GameObject Camera;
	public GameObject Sword;
	public GameObject CameraTarget1;
	public GameObject CameraTarget2;
	public GameObject CameraTarget3;
	public GameObject FeatureTarget1;
	public GameObject FeatureTarget2;
	public GameObject FeatureTarget3;

	public enum FeatureState{
		Rope = 1,
		Box = 2,
		Auto = 3,
	}

	public FeatureState featureState;

	// Use this for initialization
	void Start () {
		ChangeFeature (FeatureState.Rope);
	}

	void Reset(){
		ChangeFeature (FeatureState.Rope);

	}
	
	// Update is called once per frame
	void Update () {

#if UNITY_EDITOR
		if (Input.GetKeyDown (KeyCode.A)) {
			ChangeFeature (FeatureState.Rope);
		} else if (Input.GetKeyDown (KeyCode.S)) {
			ChangeFeature (FeatureState.Box);
		} else if (Input.GetKeyDown (KeyCode.D)) {
			ChangeFeature (FeatureState.Auto);
		}
#endif

	}

	public void ChangeFeature(FeatureState state){
		featureState = state;
		if (state == FeatureState.Rope) {
			Sword.transform.position = FeatureTarget1.transform.position;
			Sword.transform.rotation = FeatureTarget1.transform.rotation;
			Camera.transform.position = CameraTarget1.transform.position;
			Camera.transform.rotation = CameraTarget2.transform.rotation;
		} else if (state == FeatureState.Box) {
			Sword.transform.position = FeatureTarget2.transform.position;
			Sword.transform.rotation = FeatureTarget2.transform.rotation;
			Camera.transform.position = CameraTarget2.transform.position;
			Camera.transform.rotation = CameraTarget2.transform.rotation;	
		} else if (state == FeatureState.Auto) {
			Sword.transform.position = FeatureTarget3.transform.position;
			Sword.transform.rotation = FeatureTarget3.transform.rotation;
			Camera.transform.position = CameraTarget3.transform.position;
			Camera.transform.rotation = CameraTarget3.transform.rotation;	
		}
	
	}

	public void OnGUI(){
		GUI.skin.label.alignment = TextAnchor.MiddleCenter;
		GUI.skin.button.alignment = TextAnchor.MiddleCenter;
		GUI.skin.label.fontSize = 40;
		GUI.skin.button.fontSize = 15;

		if (GUI.Button(new Rect(Screen.width / 2 - 40, 5, 80, 55), "Switch")){
			if (featureState == FeatureState.Rope){
				ChangeFeature(FeatureState.Box);
			} else if (featureState == FeatureState.Box){
				ChangeFeature(FeatureState.Auto);
			} else {
				ChangeFeature(FeatureState.Rope);
			}
		}
	}

	public void OnNextScene(){
		if (featureState == FeatureState.Rope){
			ChangeFeature(FeatureState.Box);
		} else if (featureState == FeatureState.Box){
			ChangeFeature(FeatureState.Auto);
		} else {
			ChangeFeature(FeatureState.Rope);
		}
	}

	public void OnPreviousScene(){
		if (featureState == FeatureState.Rope){
			ChangeFeature(FeatureState.Auto);
		} else if (featureState == FeatureState.Auto){
			ChangeFeature(FeatureState.Box);
		} else {
			ChangeFeature(FeatureState.Rope);
		}
	}

}
