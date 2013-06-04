#include "ripples/stdafx.h"

const char * gPlugins[] = {
    "RenderSystem_GL",
    "Plugin_CgProgramManager"
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

    // Initilize rendering system

    Ogre::RenderSystem * renderSystem = root->getAvailableRenderers()[0];
    root->setRenderSystem(renderSystem);

    root->initialise(false);
    Ogre::RenderWindow * window = root->createRenderWindow("Window", 640, 480, false);

    // Load resources

    Ogre::ResourceGroupManager & resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();
    resourceGroupManager.addResourceLocation(RIPPLES_DATA_DIR, "FileSystem");
    resourceGroupManager.initialiseAllResourceGroups();
    resourceGroupManager.loadResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    // Create a scene

    Ogre::SceneManager * scene = root->createSceneManager(Ogre::ST_GENERIC);

    // Create a camera

    Ogre::Camera * camera = scene->createCamera("Camera");
    camera->setAutoAspectRatio(true);
    camera->setNearClipDistance(1.0f);
    camera->setFarClipDistance(100.0f);

    Ogre::Viewport * viewport = window->addViewport(camera);
    viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.3f, 1.0f));

    // Create a quad

    Ogre::ManualObject * quad = scene->createManualObject();
    quad->setDynamic(false);
    quad->begin("Draw", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
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
        Sleep(30);
        root->renderOneFrame();
		Ogre::WindowEventUtilities::messagePump();
	}
 
	return 0;
}
