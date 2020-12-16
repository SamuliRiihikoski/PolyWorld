New 3d modelling app. Works only linux at the moment.

HOW TO USE:  
LMB = Rotate the scene (make sure cursor is in background)  
RMB = Zoom in/out.  
E = Hover on polygon and then press E key to extrude it. Pressing LMB will complete the action.  

DOWNLOAD DEBIAN: .deb file is in debian folder.

debian install files into /home/Polyland location
  
RUN FILE: ./Polyland 3.138.155.85 8080  



v0.04

- Hover select use cpu threads. Still uses deque instead vectors.  

v0.03

- Polygon extrude (E key: LMB = apply extude, RMB = cancels extude)  
- Camera rotate and zoom  
- Polygon hovering. (Proper selection is still not working)  
- Polygon ray detection becomes very slow when there is >100 polygons on scene.   
