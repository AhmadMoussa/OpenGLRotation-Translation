Name: Ahmad Moussa
ID: 40048101
Assignment 2

1 - Upon running the program a window will open waiting for user clicks inside the window

2 - clicking on the window generates points in 3D space, note that the points can be rotated by pressing the arrow keys, and the user can switch between rendering lines and points by pressing the 'L' and 'P' buttons respectively.

3 - Hitting 'Enter' on the keyboard wil generate a smooth curve from the previously given points, this curve can also be rotated in 3D space and switched betweenr endering modes.

4 - By hitting 'Enter' while displaying the rendered curve it will take you back to seeing the input points. Pressing 'C' in that view will take you also back to seeing the input points, and pressing 'C' again will clear the window and the user can input new points.

5 - While in seeing the smooth curve the user can press 'R' to create a rotational sweep surface from the curve. However this view can only be exited by closing the window and reopening it (because I didn't have time to implement it otherwise).

6 - In the file splines.cpp is an alternative function for getting the smooth surface that uses the subdicision method but unfortunately I could not get it to work, it just produces garbage values for the given points. Instead i brute force the points and get them through the Basis and Control matrices.
