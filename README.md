<div align="center">
  <h1>🎧 Clipboard TTS Monitor</h1>
  <p><b>Una herramienta ligera y potente para Windows que lee en voz alta el contenido de tu portapapeles.</b></p>
</div>

¡Bienvenido a **Clipboard TTS Monitor**! Este es un desarrollo *Open Source* escrito en C++ (Win32 API/COM) pensado para optimizar tu flujo de trabajo, estudiar de manera pasiva y mejorar la accesibilidad de tu equipo.

## ✨ Características Principales

* **🎙️ Lectura Automática**: Simplemente copia cualquier texto (`Ctrl+C`) y el programa lo leerá en voz alta sin interrumpir tu trabajo.
* **🧠 Filtro Inteligente**: Ignora de forma silenciosa textos cortos (menos de 40 caracteres) para evitar molestias al copiar contraseñas o fragmentos pequeños.
* **📚 Cola de Reproducción**: Si copias múltiples textos largos, se encolarán de manera ordenada. El límite de la cola es configurable.
* **⚡ Atajos de Teclado Globales** (Personalizables):
  * `Ctrl + 1`: ⏭️ **Saltar** texto actual (reproduce el siguiente en la cola si existe).
  * `Ctrl + 2`: ⏯️ **Pausar / Reanudar** la lectura.
  * `Ctrl + 3`: 🔁 **Repetir** el último texto sintetizado.
  * `Alt + 1`: ⏹️ **Detener por completo** la lectura y vaciar la cola.
* **⚙️ Control Total**: Selecciona tu voz SAPI5 favorita y ajusta la velocidad de lectura (20 niveles) directamente desde el ícono en la bandeja del sistema (System Tray).
* **💾 Grabación de Audio (Opcional)**: Guarda automáticamente lo que escuchas en archivos comprimidos `.m4a` con marca de tiempo.
* **🪶 Ultra Ligero**: Construido nativamente para Windows. Consume menos de 10MB de RAM en reposo.

---

## 🚀 Instalación Rápida (Quick Install)

Para usuarios que simplemente desean utilizar la aplicación:

1. Ve a la pestaña de [Releases](../../releases) de este repositorio.
2. Descarga el instalador más reciente (`ClipboardTTSMonitor_v1.0.0.msi`).
3. Ejecuta el instalador y sigue las breves instrucciones.
4. ¡Listo! Verás un icono 🎧 en la esquina inferior derecha de tu pantalla (Bandeja del sistema). Copia un texto largo y comenzará la magia.

---

## 🛠️ Guía para Desarrolladores

¡Las contribuciones son la base de la comunidad Open Source! Si deseas colaborar, optimizar código o añadir nuevas funcionalidades, eres más que bienvenido.

### Requisitos Previos
* **Sistema Operativo**: Windows 10 / Windows 11.
* **IDE**: Visual Studio 2022 (con soporte para desarrollo de escritorio con C++).
* **Herramientas**: CMake (versión 3.20+).

### Clonar y Compilar

```bash
# 1. Clona el repositorio en tu máquina local
git clone https://github.com/alphadx/Clipboard_2_TTS.git

# 2. Ingresa al directorio del proyecto
cd Clipboard_2_TTS

# 3. Configura y compila el binario usando el preset de Ninja
cmake --preset ninja-x64-release
cmake --build --preset ninja-x64-release
```
*(Nota: El proyecto incluye archivos de configuración de CMakePresets para facilitar la compilación y trabajar directamente abriendo la carpeta en Visual Studio).*

### Empaquetado del Instalador (MSI)

Para distribuir la aplicación, el proyecto utiliza **WiX Toolset v7** para generar un instalador auto-contenido `.msi`. Una vez compilado el ejecutable, puedes generar el instalador de la siguiente manera:

```bash
# 1. Instala la extensión gráfica de WiX (si no la tienes)
wix extension add -g WixToolset.UI.wixext

# 2. Construye el paquete MSI 
wix build Package.wxs -ext WixToolset.UI.wixext -o build\ClipboardTTSMonitor.msi
```

### Conoce la Arquitectura
El software emplea un diseño técnico estructurado enfocado en la concurrencia segura y bajo consumo de recursos:
* `src/core/`: Lógica base (Escucha pasiva del portapapeles, Máquina de estados, Registro de Hotkeys).
* `src/tts/`: Integración nativa asíncrona con el motor SAPI5 (`ISpVoice`) y gestión de la cola de reproducción.
* `src/ui/`: Integración fluida con el *System Tray* de Windows.
* `src/audio/`: Módulo de codificación AAC usando *Windows Media Foundation*.

> 💡 **Tip:** Para comprender a fondo las decisiones arquitectónicas, requerimientos funcionales y reglas de desarrollo, te invitamos a leer el **Documento de Diseño Técnico (Fase 0)** ubicado en `docs/clipboard_tts_monitor_fase0.html` y los updates en `docs/modificaciones.md`.

---

## 🤝 Cómo Colaborar

¿Tienes una idea para mejorar el programa? ¡Hazla realidad!

1. Haz un **Fork** de este repositorio.
2. Crea tu rama para la nueva funcionalidad (`git checkout -b feature/MejoraIncreible`).
3. Realiza tus cambios asegurándote de seguir el estándar del proyecto (punteros inteligentes RAII, soporte Unicode, etc.).
4. Haz **Commit** de tus cambios (`git commit -m 'Añade una mejora increible'`).
5. Sube tus cambios a tu rama (`git push origin feature/MejoraIncreible`).
6. Abre un **Pull Request** y revisaremos tu aporte lo más pronto posible.

---

## 🐛 Reporte de Errores y Soporte

Si encuentras algún *bug* o tienes alguna sugerencia, por favor abre un [Issue](../../issues) detallando el problema. ¡El feedback es invaluable!

## 📜 Licencia

Este proyecto está bajo filosofía de código abierto. Siéntete libre de utilizarlo, modificarlo y compartirlo para hacer la accesibilidad digital cada vez mejor.
