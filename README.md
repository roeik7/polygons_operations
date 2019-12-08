
Markup :  # Polygons operations #

+ **polygons_operations**

+ **Utilized: Linux, C.**

This program gets file with commands related to polygons (add new polygon (hexagon/heptagon/octagon), execute operation on specific group of polygons (newest polygon, all polygons, only hexagons .etc.

 **ex2_q1**
 Get file with commands and separate for 3 files, then execute ex2_q1_helper to execute the oprations in parrallel (for each file we make fork and exec).
 Each file contain commands related for its polygons (file for hexagons, heptagons, octagons).
 
**ex2_q1_helper**
executed by ex2_q1 program. each file from the input file will run by another process.

