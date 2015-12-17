# 2D analytical geometry
First of the three university projects for computer graphics. 2D analytic geometry, splines.

//TODO: translate project description

Magyar feladatleírás:

Adott egy 1000m x 1000m-res mező, amelyet kezdetben a teljes viewport-ban látunk. Az egér bal gomb lenyomásával pontokat helyezünk el a mezőn. A pontokat piros belsejű, fehér körvonalú 5m sugarú körrel jelenítjük meg. Egyrészt a pontok egy fehér, önmagában zárt Catmull-Rom spline kontrollpontjai, amelyekhez tartozó időparaméter a lenyomáskori óra állás. Másrészt az első két pont egy egyenest definiál, a harmadik pedig egy fókusz pontot. A egyenes és fókuszpont által kijelölt parabola a síkot két részre osztja. A parabola fókuszpontot tartalmazó részében a hátteret sárgára, a másik részben türkizre színezzük ki. A Catmull-Rom spline és a parabola egyik metszéspontjában (már ha van) zöld színnel érintő egyenest rajzolunk mind a parabolához, mind pedig a spline-hoz.

A SPACE lenyomásának hatására a kamera ablak 500m x 500m-resre megy össze úgy, hogy a középpontja helyben marad. Ezután (2m/sec,3m/sec) kezdeti sebességgel vándorútra indul. Ha a kameraablak ütközik a mező szélével, onnan biliárd golyó módjára verődik vissza.

Minden, a feladat szövege által nem specifikált részlet szabadon megválasztható.
