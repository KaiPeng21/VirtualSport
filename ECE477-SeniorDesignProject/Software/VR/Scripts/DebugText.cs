using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DebugText : MonoBehaviour {

	private TextMesh textObject;

	public static bool IsDebugging;
	public static string Text;

	// Use this for initialization
	void Start () {
		IsDebugging = true;
		textObject = gameObject.GetComponent<TextMesh> ();
	}
	
	// Update is called once per frame
	void Update () {
		if (IsDebugging) {
			textObject.text = Text;
		} else {
			textObject.text = ""; 
		}
	}

	public static void SetText(string text){
		Text = text;
	}
}
