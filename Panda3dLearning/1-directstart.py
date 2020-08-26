'''
DirectStart takes care of everything needed to start up Panda3D by creating the

architecture that is necessary for Panda3D to run. This includes creating a default Panda3D window, the graphics pipe for the window, the renderer, the task manager, and more. To put it simply, DirectStart makes Panda3D go.

'''

import direct.directbase.DirectStart

'''
This call starts up the components created by DirectStart so that the visualization can operate. Without run(), the utility that controls all of Panda3D's actions, each frame wouldn't start. Panda3D relies on a main loop that performs several important operations, including processing graphics and rendering frames. Each time this main loop executes, it performs these functions, renders a frame, and then repeats.
'''
run()
