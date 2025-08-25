# 🐚 Minishell Comprehensive Test Suite

Este conjunto de scripts de prueba está diseñado para testear exhaustivamente tu minishell con todos los casos límite, casos extremos y escenarios de estrés imaginables. Si tu minishell pasa todas estas pruebas, estará preparada para cualquier evaluación.

## 📋 Scripts Incluidos

### 1. `run_all_tests.sh` - Script Maestro 🎯
**El script principal que debes ejecutar**
- Ejecuta todos los test suites en orden
- Proporciona un reporte final completo
- Maneja timeouts y errores gracefully

```bash
# Ejecución básica (recomendado)
./run_all_tests.sh

# Modo rápido (solo tests esenciales)
./run_all_tests.sh --quick

# Saltar tests de memoria
./run_all_tests.sh --skip-memory

# Modo verbose
./run_all_tests.sh --verbose

# Ver ayuda
./run_all_tests.sh --help
```

### 2. `comprehensive_test.sh` - Tests Funcionales Completos 🧪
Tests sistemáticos de **TODA** la funcionalidad básica:
- ✅ Comandos builtin (echo, cd, pwd, export, unset, env, exit)
- ✅ Expansión de variables ($VAR, $?, $$)
- ✅ Manejo de comillas (simples, dobles, mixtas)
- ✅ Pipes (simples, múltiples, complejos)
- ✅ Redirecciones (<, >, >>, 2>)
- ✅ Heredocs (<<)
- ✅ Manejo de errores y códigos de salida
- ✅ Variables de entorno
- ✅ Casos edge normales

**Casos cubiertos:** ~200+ tests
**Tiempo estimado:** 2-5 minutos

### 3. `extreme_stress_test.sh` - Casos Extremos y Parser Stress 💥
Tests diseñados para **ROMPER** tu parser:
- 🔥 Inputs maliciosos y sintaxis inválida
- 🔥 Comillas sin cerrar, anidadas, caóticas
- 🔥 Redirecciones imposibles y pipe hell
- 🔥 Variables con caracteres especiales
- 🔥 Comandos extremadamente largos
- 🔥 Caracteres unicode, binarios, control
- 🔥 Paths corruptos y entornos rotos
- 🔥 Recursión y anidamiento profundo
- 🔥 Nombres de archivo imposibles

**Casos cubiertos:** ~100+ extreme cases
**Tiempo estimado:** 3-8 minutos

### 4. `memory_test.sh` - Memory Leaks y Performance 🧠
Tests de memoria y rendimiento:
- 🔍 Detección de memory leaks (con Valgrind si está disponible)
- 🔍 Tests de performance y benchmarking
- 🔍 Ejecución concurrente y stress
- 🔍 Monitoreo de uso de recursos
- 🔍 Tests de sesiones largas
- 🔍 Manejo de memoria bajo presión

**Tiempo estimado:** 1-3 minutos (más con Valgrind)

## 🚀 Uso Rápido

1. **Compilar minishell:**
```bash
make
```

2. **Ejecutar todos los tests:**
```bash
./run_all_tests.sh
```

3. **Si algo falla, ejecutar tests individuales:**
```bash
# Solo tests básicos
./comprehensive_test.sh

# Solo casos extremos  
./extreme_stress_test.sh

# Solo memory leaks
./memory_test.sh
```

## 📊 Interpretando los Resultados

### ✅ Verde = PASS
- El test pasó correctamente
- Tu minishell maneja este caso bien

### ❌ Rojo = FAIL  
- El test falló
- **¡Hay que arreglar esto!**
- Se muestra output esperado vs actual

### ⚠️ Amarillo = WARNING
- Test ejecutado pero con notas
- Generalmente no es crítico

## 🎯 Casos de Uso por Escenario

### Desarrollo Activo 👨‍💻
```bash
# Tests rápidos durante desarrollo
./run_all_tests.sh --quick

# Solo funcionalidad básica
./comprehensive_test.sh
```

### Pre-Evaluación 🎓
```bash
# TODOS los tests (puede tardar 10-15 min)
./run_all_tests.sh --verbose

# Con memory leak detection
# (requiere Valgrind: brew install valgrind)
./memory_test.sh
```

### Debug de Problema Específico 🐛
```bash
# Si fallan los básicos
./comprehensive_test.sh

# Si el parser crashea con inputs raros
./extreme_stress_test.sh  

# Si hay memory leaks
./memory_test.sh
```

## 🛡️ Qué Hacen Estos Tests Especiales

### Tests de Parsing Extremo:
- Inputs que han roto otros minishells
- Sintaxis inválida pero no debería crashear
- Edge cases que los correctores usan

### Tests de Memory:
- Detecta leaks que Valgrind encuentra
- Mide performance bajo stress
- Verifica que la limpieza sea correcta

### Tests de Robustez:
- ¿Qué pasa si PATH está vacío?
- ¿Y si mandamos null bytes?
- ¿Si usamos archivos con nombres imposibles?
- ¿Si anidamos 1000 pipes?

## 📈 Objetivo de Robustez

Si tu minishell pasa **TODOS** estos tests:

✅ Maneja todos los casos del subject correctamente  
✅ No crashea con inputs maliciosos  
✅ No tiene memory leaks significativos  
✅ Maneja errores gracefully  
✅ Performance aceptable bajo stress  
✅ **Lista para cualquier evaluación**  

## 🔧 Troubleshooting

### "Command not found" en tests
```bash
# Asegúrate de que minishell esté compilado
make
ls -la ./minishell  # debe existir y ser ejecutable
```

### Tests muy lentos
```bash
# Usar modo rápido
./run_all_tests.sh --quick

# O saltar memory tests
./run_all_tests.sh --skip-memory
```

### Memory tests no funcionan
```bash
# Instalar Valgrind en macOS
brew install valgrind

# En Linux
sudo apt-get install valgrind
```

### Test específico falla
```bash
# Ejecutar solo ese test para debug
./comprehensive_test.sh | grep -A 5 -B 5 "nombre_del_test"
```

## 💡 Tips para Desarrolladores

1. **Ejecuta `./run_all_tests.sh --quick` frecuentemente** durante el desarrollo
2. **Arregla fallos inmediatamente** - no los acumules
3. **Presta especial atención a memory leaks** - son críticos en 42
4. **Si algo crashea en extreme tests**, revisa tu parser
5. **Usa Valgrind localmente** antes de entregar

## 🏆 Niveles de Robustez

- **Level 1**: Pasa comprehensive tests ➡️ "Funciona básicamente"
- **Level 2**: Pasa extreme tests ➡️ "Parser robusto"  
- **Level 3**: Pasa memory tests ➡️ "Memory management sólido"
- **Level 4**: Pasa TODO ➡️ "**MINISHELL BULLETPROOF**" 🛡️

---

**¡Que tu minishell sea tan robusta que ningún corrector la pueda romper!** 🚀
