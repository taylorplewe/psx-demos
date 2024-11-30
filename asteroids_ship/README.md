![asteroids_ship](https://github.com/user-attachments/assets/67622eb4-8898-4d80-9432-b4053f7bed13)

Just the [Fixed point Math](http://lameguy64.net/tutorials/pstutorials/chapter1/5-fixedpoint.html) section of lameguy64's tutorial, an Asteroids-like ship.

___Switch conttroller to Analog mode to control the ship's 360&deg; movement with the left stick and 360&deg; direction with the right stick.___

This demo taught me about:
- Drawing 3-sided polygons
- Rotating said polygons using basic trigonometry (cosine, sine)
- Combining the analog joystick's positions with trigonomotry functions to achieve 360&deg; turning and movement
  - A "fast atan2" method from [a Legacy of Kain: Soul Reaver disassembly](https://github.com/FedericoMilesi/soul-re/blob/8d859e8a3885e8c57f51e42bdb299fa2180258cc/src/Game/MATH3D.c#L132) was used
