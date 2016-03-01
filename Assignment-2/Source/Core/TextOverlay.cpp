#include "TextOverlay.h"

TextOverlay::TextOverlay(Ogre::String nme, float x, float y, float w, float h) : name(nme) {
	olm = Ogre::OverlayManager::getSingletonPtr();
	panel = static_cast<Ogre::OverlayContainer*>(olm->createOverlayElement("Panel", name + "GUI"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, 0);
	panel->setDimensions(w, h);
	overlay = olm->create(name);
	overlay->add2D(panel);
	overlay = olm->getByName(name);
	panel = static_cast<Ogre::OverlayContainer*>(olm->getOverlayElement(name + "GUI"));
	textArea = static_cast<Ogre::TextAreaOverlayElement*>(olm->createOverlayElement("TextArea", name));
	panel->addChild(textArea);
	this->setPos(x, y);
	this->showOverlay();
	//Color->setCol(1.0f, 1.0f, 1.0f, 1.0f);
}

TextOverlay::~TextOverlay(void) {
	olm->destroyOverlayElement(name);
	olm->destroyOverlayElement(name + "GUI");
	olm->destroy(name);
}

void TextOverlay::setText(Ogre::String str) {
	textArea->setCaption(str);
	textArea->setDimensions(1.0f, 1.0f);
	textArea->setMetricsMode(Ogre::GMM_RELATIVE);
	textArea->setFontName("dameron");
	textArea->setCharHeight(0.03f);
}

void TextOverlay::setPos(float x, float y) {
	textArea->setPosition(x, y);
}

void TextOverlay::setCol(float r, float g, float b, float a) {
	textArea->setColour(Ogre::ColourValue(r, g, b, a));
}

void TextOverlay::showOverlay(void) {
	overlay->show();
}