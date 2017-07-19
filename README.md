# Gotas

Este proyecto consiste en una colección de rutinas en C++ que permiten analizar la #morfología de gotas en secuencias de imágenes de patrones de gotas obtenidas con una cámara digital a través de una microscopio óptico. El programa solicita al usuario que introduzca el nombre de una imagen de referencia y que indique el número final de la secuencia de imágenes hasta donde se requiere realizar el análisis. En principio, los resultados que arroja el programa principal son los valores de los radios de gotas y un histograma de distribución de tamaños. Además, los histogramas de la secuencia de gotas a partir de la imagen de referencia pueden ser estudiados a través de la ecuación de coagulación de Smoluchowski, la cual se encuentra representada por el cálculo de coeficientes hasta orden 3. 

Las rutinas usan librerías de OpenCV 2.4 y librerías estándar de C++. 
