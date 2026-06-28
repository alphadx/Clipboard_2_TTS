# Modificaciones al Reproductor de Portapapeles

## Nuevas Características y Cambios (Junio 2026)

1. **Filtro de Longitud de Texto**:
   - Ahora el programa sólo lee textos del portapapeles que contengan **más de 40 caracteres**.
   - Los textos más cortos son ignorados silenciosamente para evitar molestias.

2. **Gestión de Cola de Reproducción**:
   - Se ha añadido una cola para las reproducciones de texto.
   - Mientras un texto se está reproduciendo, los nuevos textos copiados (que superen los 40 caracteres) se encolarán hasta un máximo de **N** elementos.
   - El límite de la cola (N) se puede configurar desde la interfaz gráfica (menú contextual) con valores entre 1 y 10.
   - El valor por defecto para el tamaño de la cola es **1**.

3. **Nuevos Atajos de Teclado**:
   - **`Ctrl + 1`**: Detiene la reproducción actual (saltar). Si hay elementos en la cola, continuará automáticamente con la reproducción del siguiente texto.
   - **`Alt + 1`**: Detiene completamente todas las reproducciones actuales y vacía la cola. El programa queda en estado de escucha esperando nuevos textos.
   - **`Ctrl + 2`**: Pausa la reproducción actual. Al presionarlo nuevamente, reanuda la reproducción desde el mismo punto en el que se pausó.

4. **Cambios Internos Relevantes**:
   - El `TTSEngine` ahora gestiona internamente la cola (`m_textQueue`) y el estado de pausa.
   - Se capturan eventos de la API SAPI (`WM_APP_TTS_EVENT` / `SPEI_END_INPUT_STREAM`) para pasar automáticamente al siguiente elemento de la cola cuando finaliza un texto.
   - El `CConfigManager` fue actualizado para persistir las nuevas configuraciones y los atajos de teclado asignados.
