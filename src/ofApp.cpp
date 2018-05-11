#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
#endif

	mVideoGrabber.setup(600, 350);
	mVideoGrabber.setVerbose(true);

    img.allocate(mVideoGrabber.getWidth(), mVideoGrabber.getHeight(), OF_IMAGE_GRAYSCALE);
	mGrayscaleImage.allocate(mVideoGrabber.getWidth(), mVideoGrabber.getHeight());
	mColorImage.allocate(mVideoGrabber.getWidth(), mVideoGrabber.getHeight());
	grayDiff.allocate(mVideoGrabber.getWidth(), mVideoGrabber.getHeight());
	mBackground.allocate(mVideoGrabber.getWidth(), mVideoGrabber.getHeight());
    plane.set(2000, 1300, mVideoGrabber.getWidth()/3, mVideoGrabber.getHeight()/3);
    plane.mapTexCoordsFromTexture(img.getTexture());
}

//--------------------------------------------------------------
void ofApp::update(){
	mVideoGrabber.update();

	if (mVideoGrabber.isFrameNew()) {
		mColorImage.setFromPixels(mVideoGrabber.getPixels());
		if (ofGetElapsedTimef() < 1.0f) {
			mBackground = mColorImage;
		}
		//this equal sign does NOT set the grayscale image to equal the color image, it has been overloaded to mean JUST pull the grayscale
		mGrayscaleImage = mColorImage;
		//take the absolute value of the difference of two pixels
		grayDiff.absDiff(mGrayscaleImage, mBackground);
	}

	grayDiff.threshold(100);
	

    float noiseScale = 0.01;
    float noiseVel = ofGetElapsedTimef();
	ofPixels pixels = img.getPixels();
	ofPixels mGrayscalePixels = mGrayscaleImage.getPixels();
    int w = img.getWidth();
    int h = img.getHeight();
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;

			if (mGrayscalePixels[i] > 100) {
				noiseScale = 0.08;
				float noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
				pixels[i] = 100 * noiseValue + mGrayscalePixels[i];
			}

			else {
				noiseScale = 0.01;
				float noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
				pixels[i] = 50 * noiseValue + mGrayscalePixels[i];
			}

            
        }
    }
	mContourFinder.findContours(grayDiff, 10, 1000, 10, true, true);
	img.setFromPixels(pixels);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
    // bind our texture. in our shader this will now be tex0 by default
    // so we can just go ahead and access it there.
    img.getTexture().bind();
    
	int max = 0;
	for (auto blob : mContourFinder.blobs){
		if (blob.area > max) {
			max = blob.area;
			brightest_spot_x = blob.centroid.x;
			brightest_spot_y = blob.centroid.y;
		}
	}

    shader.begin();
	shader.setUniform2f("mousePos", brightest_spot_x, brightest_spot_y);
    ofPushMatrix();
    
    // translate plane into center screen.
    float tx = ofGetWidth() / 2;
    float ty = ofGetHeight() / 2;
    ofTranslate(tx, ty);

    // the mouse/touch Y position changes the rotation of the plane.

    ofRotateY(sin(ofGetElapsedTimef()));
	ofRotateX(sin(ofGetElapsedTimef()));

    plane.drawWireframe();
	
    ofPopMatrix();
    
    shader.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
