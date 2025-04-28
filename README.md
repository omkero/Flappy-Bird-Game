[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://opensource.org/license/gpl-3-0)



# Astro Ship

Flappy Bird its Game built using C/C++ with SDL2 library



## 📦 Installation & Build Instructions  

### **1️⃣ Prerequisites**  
Make sure you have the following installed:  
- **C++17 or later**  
- **SDL2 (lateset version)**  
- **CMake 3.14+**


### **2️⃣ Clone the Repository**  
```sh
git clone https://github.com/omkero/Flappy-Bird-Game.git

```


### **3️⃣ Install Required SDL2 libraries**  
On Windows, SDL2 libraries should be located in lib/x64, and CMake will automatically link them to the executable. 

## (Linux Only)
```sh
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev

```




### **4️⃣  Build and run Using CMake**

Before running the game, make sure to copy the assets/ folder and place it in the same directory as the executable. Otherwise, the game will not run correctly.

```sh
cd flappy

cmake -S . -B build/

cmake --build build/ --config Release
```

## Run on Windows
```sh
./build/Release/flappy_bird.exe

```

## Run on Linux
```sh
./build/flappy_bird 

```






## 🤝 Contributions

I welcome any contributions! Feel free to fork the repository, make improvements, and submit a pull request. Whether it's bug fixes, new features, or performance optimizations, all contributions are appreciated.



## Enjoy the game! 


📝 License
This project is open-source and available under the GPL3 License.

Let me know if you need any more adjustments 😃!

- [@omkero](https://www.github.com/omkero)

