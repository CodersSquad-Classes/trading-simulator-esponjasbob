# Documentación del Proyecto: Stock Exchange Matching Engine (simulador)

## Introducción

Este proyecto implementa un motor de casamiento (matching engine) de un libro de órdenes límite (Limit Order Book) para una sola acción, con una interfaz de terminal que actúa como un "dashboard" en tiempo real. A diferencia de una versión interactiva, el ejecutable en `main.cpp` inyecta órdenes aleatorias en el libro cada segundo para simular actividad de mercado.

Conceptos básicos:

- Bids (compras): órdenes límite para comprar a distintos precios y cantidades.
- Asks (ventas): órdenes límite para vender a distintos precios y cantidades.

La prioridad del libro es por precio y, a igualdad de precio, por tiempo (price-time priority): mejor precio primero; a igual precio, la orden con `seq` menor (más antigua) se prioriza.

## Investigación sobre Limit Order Books

Un Limit Order Book es la estructura central de muchos mercados electrónicos. Conceptos clave:

- Orden límite: instrucción para comprar/vender una cantidad a un precio límite (o mejor).
- Mejor bid: mayor precio de compra disponible en el libro.
- Mejor ask: menor precio de venta disponible en el libro.
- Spread: diferencia entre mejor ask y mejor bid.
- Prioridad precio-tiempo: primero mejor precio; a igual precio, se ejecuta la orden con timestamp/`seq` más antiguo.

En la práctica, los libros se actualizan continuamente, mostrando profundidad por niveles de precio. El código de este repositorio implementa una versión simplificada que facilita su estudio.

## Diseño y Arquitectura

### Modelo de datos

Estructura `Order` (ver `Order.h`):

- `price` (double): precio límite de la orden.
- `shares` (int): cantidad de acciones.
- `seq` (int): secuencia incremental global para romper empates por tiempo.
- `counter` (static int): contador global que asigna `seq` al construir una orden (definido en `Order.cpp`).

Suposiciones sencillas: los inputs generados por el simulador son válidos. Se puede extender con validaciones si es necesario.

### Estructuras de datos

Se usan `std::priority_queue<Order, vector<Order>, Comparator>` para ambos lados:

- Cola de compras (`buy_orders`) con `BuyComparator`: ordena por mayor precio primero; a igualdad de precio, menor `seq` primero (más antiguo).
- Cola de ventas (`sell_orders`) con `SellComparator`: ordena por menor precio primero; a igualdad de precio, menor `seq` primero (más antiguo).

Esto implementa la prioridad precio-tiempo de forma eficiente para acceder siempre al top-of-book.

### Contrato de matching

- Condición de cruce: existe trade si `bestBuy.price >= bestSell.price`.
- Cantidad ejecutada: `min(buy.shares, sell.shares)`.
- Precio de ejecución: en la implementación actual se toma `sell.price` (lado pasivo).
- Órdenes residuales: si queda cantidad en buy o sell tras el trade, la orden residual se vuelve a insertar manteniendo su `seq`.

Diagrama de flujo (simplificado):

```
Nueva orden (aleatoria en runtime) -> addBuy/addSell -> match()
    while bestBuy && bestSell:
        if bestBuy.price < bestSell.price: break
        traded = min(buy.shares, sell.shares)
        registrar trade: "<traded> @ <price>" (price = sell.price)
        actualizar cantidades y reinsertar si queda remanente
```

### Interfaz en tiempo real (terminal)

El programa muestra un dashboard que se actualiza cada segundo y contiene:

- Último precio (`Last Price`) con un indicador de movimiento (▲ en verde si sube, ▼ en rojo si baja).
- Estadísticas: número total de trades (`Trades`), volumen total (`Volume`) y spread actual (`Spread`).
- Lista de "Recent Trades" (tape) con hasta 5 entradas, formato `"<qty> @ <price>"`.
- Snapshot del libro de órdenes: `printBook()` muestra hasta 10 niveles por lado (columnas `BUY Qty | BUY Px  | SELL Qty | SELL Px`).

Notas de implementación:

- `main.cpp` limpia la pantalla y re-dibuja el dashboard en un bucle infinito; la versión actual no solicita entrada manual.
- La función `printBook()` no aplica colorización por lado; sólo `main.cpp` usa códigos ANSI para indicar la dirección del último precio.

### Decisiones y limitaciones

- Simplicidad vs. fidelidad: se prioriza un diseño claro con `priority_queue` y comparadores personalizados.
- Un solo símbolo: el proyecto maneja una única instancia de `LimitOrderBook`; puede extenderse con `unordered_map<string, LimitOrderBook>` para múltiples símbolos.
- Entrada automática: el ejecutable añade órdenes aleatorias en cada iteración; para entrada manual hay que modificar `main.cpp` o proporcionar una versión alternativa.

## Implementación

### Clases y funciones clave

- `class Order` (`Order.h` / `Order.cpp`):
    - Campos: `price`, `shares`, `seq`.
    - Constructor: asigna `seq = counter++`.

- Comparadores (`Order.h`):
    - `BuyComparator`: mayor `price` primero; a igualdad, menor `seq`.
    - `SellComparator`: menor `price` primero; a igualdad, menor `seq`.

- `class LimitOrderBook` (`LimitOrderBook.h` / `.cpp`):
    - `addBuy(price, qty)` y `addSell(price, qty)`: insertan y llaman a `match()`.
    - `match()`: ejecuta trades mientras haya cruce; actualiza `lastPrice`, `totalVolume`, `totalTrades` y registra una entrada en la cinta (`tape`) con el formato `"<qty> @ <price>"`. La cinta guarda hasta 5 últimas entradas.
    - `printBook() const`: muestra snapshot del top-of-book (hasta 10 niveles por lado) sin mutar el estado.

- `main.cpp`:
    - Bucle principal: limpia pantalla, muestra estadísticas, imprime tape y libro, y añade una orden aleatoria cada segundo mediante `addRandomOrder()`.

## Uso y comportamiento

Compilación:

```
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Order.cpp LimitOrderBook.cpp -o orderbook
```

Ejecución:

```
./orderbook
```

Comportamiento en runtime:

- El programa corre en bucle infinito y, cada segundo, genera una orden aleatoria (compra o venta) con precio uniformemente aleatorio entre `23.0` y `25.0` y cantidad entre `1` y `6000`.
- La UI no espera comandos del usuario en la versión actual; para detener la ejecución se debe interrumpir el proceso (Ctrl+C) o modificar `main.cpp`.

Si quieres cambiar la entrada para que sea manual (menu interactivo), puedo ayudarte a adaptar `main.cpp` para leer comandos `b`/`s`/`p`/`q` en lugar de inyectar órdenes aleatorias.

## Pruebas sugeridas

Dado que la versión actual genera órdenes aleatorias, puedes validar comportamientos observando el dashboard durante unos segundos y comprobando:

1) Que `getTrades()` y `getVolume()` aumentan cuando ocurren crosses.
2) Que `Last Price` se actualiza con el precio de las ventas que cruzan (`sell.price`).
3) Que la cinta (`Recent Trades`) muestra entradas con el formato `"<qty> @ <price>"` y guarda hasta 5 trades.

Si prefieres pruebas determinísticas, puedo preparar un `main_manual.cpp` que inserte órdenes conocidas en secuencia y así verificar casos concretos (cruce total, cruce parcial y prioridad por tiempo).

## Conclusiones y Aprendizajes

La práctica permitió construir un simulador sencillo de matching engine que usa `std::priority_queue` con comparadores personalizados para mantener prioridad precio–tiempo y gestionar fills parciales correctamente. Además se añadió una vista en terminal que facilita observar la dinámica del libro y los trades. La implementación es intencionalmente simple y puede extenderse para manejar múltiples símbolos, entrada manual o persistencia de eventos.



