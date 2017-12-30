#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp() {
	for (int i = this->particles.size() - 1; i > -1; i--) {
		delete this->particles[i];
		this->particles.erase(this->particles.begin() + i);
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofNoFill();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {
	Leap::Frame frame = leap.frame();
	for (Leap::Hand hand : frame.hands()) {
		for (Leap::Finger finger : hand.fingers()) {
			ofVec2f velocity = ofVec2f(finger.tipVelocity().x, -finger.tipVelocity().y);
			if (velocity.length() > 250) {
				ofVec3f finger_position = ofVec3f(ofMap(finger.tipPosition().x, -300, 300, -ofGetWidth() / 2, ofGetWidth() / 2),
					ofMap(finger.tipPosition().y, 0, 300, -ofGetHeight() / 2, ofGetHeight() / 2),
					finger.tipPosition().z);
				this->particles.push_back(new Particle(finger_position, velocity * 0.008));
			}
		}
	}

	for (int i = this->particles.size() - 1; i > -1; i--) {
		this->particles[i]->update();

		if (this->particles[i]->isDead()) {
			delete this->particles[i];
			this->particles.erase(this->particles.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	this->cam.begin();

	ofSetColor(255, 128);
	ofDrawBox(720);

	float distance;
	for (int i = 0; i < this->particles.size(); i++) {
		this->particles[i]->draw();
		for (int j = i; j < this->particles.size(); j++) {
			distance = this->particles[i]->get_location().distance(this->particles[j]->get_location());
			if (distance < 80) {
				ofDrawLine(this->particles[i]->get_location(), this->particles[j]->get_location());
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}