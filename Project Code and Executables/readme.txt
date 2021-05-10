Code Execution: All the executables are shared in this folder.
• moisture_metal_module: this folder has moisture_metal_module.ino file that has to be uploaded to first Arduino and executed on that to control moisture and metal detection modules
• glass_plastic_module: this folder contains glass_plastic_module.ino file that has to be uploaded to second Arduino to control glass or plastic detection module.
• client.py: This file is to be run on a local laptop and the laptop needs to be connected with second Arduino to enable capturing of the images. 
Command: python client.py
• Ipynb file: GlassOrPlasticClassifier.ipynb file has the image classifier model code in it. It should be executed on google colaboratory and the dataset should be uploaded to google drive and the drive should be mounted to the colaboratory notebook.
