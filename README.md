# Tarea3Ensambla
Adrian Arias Vargas C30749
Juan Sebastián Loaiza Ospina B74200
Sergio Solano Mora C37633



## Requisitos
SFML
Se utilizo para hacer la interfaz grafica, se instala con:
- sudo apt install libsfml-dev


Text-to-Speech con pico2wave

Se utilizo pico2wave para leer en voz alta la respuesta. Es importante aclarar que solo es capaz de leer en ingles.

- sudo apt install libttspico-utils alsa-utils

Whisper Transcripcion de audio a texto

La no se incluye en el git debido a su tamaño, pero se puede instalar con:

- git clone https://github.com/ggerganov/whisper.cpp
- cd whisper.cpp
- make

- ./models/download-ggml-model.sh base.en

El binario whisper-cli debe estar accesible desde ./whisper/whisper-cli
Y que el modelo esté en: ./whisper/models/ggml-base.en.bin

Lo cual se veria algo asi:
whisper
├── models/ ggml-base.en.bin
└── whisper-cli

Backend con Ollama

Para installar Ollama

1. Se descarga Ollama con:
    curl -fsSL https://ollama.com/install.sh | sh
2. Se descarga el modelo llama3:
    ollama pull llama3 (pesa aproximadamente 4.7GB)

La app espera que esté corriendo un servidor en:

http://localhost:3000/api/assembly/recognizer
Por lo que si el puerto 3000 esta ocupado no va a ser capaz de correr.


Ollama funciona con un JSON como este:

{
  "prompt": "¿La Universidad de Costa Rica ofrece becas?",
  "model": "llama3",
  "language": "ensamblador"
}

Y responder con algo como:

{
  "data": {
    "response": "Si, la Universidad de Costa Rica..."
  }
}

El programa utiliza la Ley de Amdahl para mostrar el impacto de:

- Transcripción de audio
- Procesamiento del backend
- Sintesis de voz

La cual se imprime en consola mostrando:
======= Performance Measurement =======
Transcription time      : N ms
Backend processing time : N ms
Speech synthesis time   : N ms
Total time              : N ms
Parallelizable fraction (P): X
Available cores (N)     : X
Approximate speedup     : Nx
=======================================


El proyecto esta definido de la siguiente manera:
    
Tarea3Ensambla (AssemblerVoiceAssistant)
├── asm/ ← Parte en ensamblador
├── resources/ ← aqui se manejan los recursos para la interfaz grafica como imagenes, ademas de la 
|                grabacion.wav y la transcripcion
├── src/ ← C++ GUI + logica
├── include/ ← headers
├── whisper/ ← whisper-cli y modelos
├── ollama/ ← backend Node.js + rutas
├── Makefile ← makefile para correr el programa
└── bin/ ← ejecutable generado


-Como ejecutar-
Make run: ejecuta solo la interfaz grafica
Make run-full: ejecuta la interfaz junto al backend
Make clean: elimina las carpetas obj y bin
