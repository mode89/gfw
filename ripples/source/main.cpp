#include "ripples/stdafx.h"

const char * gPlugins[] = {
    "RenderSystem_GL"
};

int main()
{
    Ogre::SharedPtr<Ogre::Root> root(new Ogre::Root());

    // Load plugins

    for (int i = 0; i < sizeof(gPlugins) / sizeof(gPlugins[0]); ++ i)
    {
        Ogre::String plugin = gPlugins[i];
#if OGRE_DEBUG_MODE
        plugin += "_d";
#endif
        root->loadPlugin(plugin);
    }

    Ogre::RenderSystem * renderSystem = root->getAvailableRenderers()[0];
    root->setRenderSystem(renderSystem);

    root->initialise(false);
    Ogre::RenderWindow * window = root->createRenderWindow("Window", 640, 480, false);

    Ogre::SceneManager * scene = root->createSceneManager(Ogre::ST_GENERIC);

    Ogre::Camera * camera = scene->createCamera("Camera");
    camera->setNearClipDistance(1.0f);
    camera->setFarClipDistance(100.0f);

    Ogre::Viewport * viewport = window->addViewport(camera);
    viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.3f, 1.0f));

    Ogre::ManualObject * quad = scene->createManualObject();
    quad->setDynamic(false);
    quad->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
    {
        quad->position(-1.0f, 1.0f, 0.0f);
        quad->position(-1.0f, -1.0f, 0.0f);
        quad->position(1.0f, 1.0f, 0.0f);
        quad->position(1.0f, -1.0f, 0.0f);
    }
    quad->end();

    Ogre::SceneNode * node = scene->getRootSceneNode()->createChildSceneNode();
    node->attachObject(quad);
    node->translate(0.0f, 0.0f, -3.0f);

	while(!window->isClosed())
	{
        window->update(true);
        root->renderOneFrame();
		Ogre::WindowEventUtilities::messagePump();
	}
 
	return 0;
}
