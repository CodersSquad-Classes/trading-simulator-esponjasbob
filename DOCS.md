# Evidence 1.7.1 - Trading Simulator

Este proyecto es un simulador sencillo de un **Limit Order Book (LOB)** para una sola acción. Todo corre en la terminal y cada segundo se generan órdenes aleatorias para imitar un mercado en vivo.

## ¿Qué es un Limit Order Book?

Un LOB mantiene dos tipos de órdenes:

- **Bids (compras)**: quieren comprar a un precio.
- **Asks (ventas)**: quieren vender a un precio.

Reglas básicas:
- Mejor precio tiene prioridad.
- Si dos órdenes tienen el mismo precio, se usa el orden de llegada.
- Hay trade si *bestBid >= bestAsk*.

## Diseño del proyecto

### Ordenes
Cada orden tiene:
- `price`
- `shares`
- `seq` (contador para decidir cuál llegó primero)

### Estructuras de datos
Usamos dos `priority_queue`:

- **buy_orders**: ordena por mayor precio primero.
- **sell_orders**: ordena por menor precio primero.

Esto simula la prioridad precio–tiempo.

### Matching
Cuando se agrega una orden (sea buy o sell), se llama a `match()`:

1. Se comparan el mejor buy y el mejor sell.
2. Si cruzan, se ejecuta un trade:
   - Cantidad = `min(buy, sell)`
   - Precio = precio del sell (lado pasivo)
3. Si queda remanente, se reinsertan las órdenes con su mismo `seq`.
4. Se guarda el trade en la lista de “Recent Trades”.

### Interfaz en consola
Cada segundo se imprime:

- Último precio con indicador de subida/bajada.
- Número total de trades y volumen.
- Spread.
- Últimos 5 trades.
- El libro de órdenes (10 niveles por lado).

Todo se redibuja limpiando la terminal.

## Cómo correrlo

Compilar:
```
g++ -std=c++17 main.cpp Order.cpp LimitOrderBook.cpp -o orderbook
```

Ejecutar:
```
./orderbook
```

## Conclusiones y Aprendizajes

- **Ricardo**: 

En este projecto aprendí a simular un *matching engine* para un libro de órdenes continua, utilizando como principal característica *priority queues*. Para hacer el programa más efectivo, también aprendí de ciertas "costumbres" para organizar mejor los archivos tanto `.cpp` como `.h`. Por último, aprendí qué son los libros de órdenes y cómo funcionan en el mundo del intercambio de bolsas. Concluyo, que esta práctica fue una buena manera para visualizar las colas con prioridad ya que las órdenes de intercambio se organizan de acuerdo a alguna prioridad, así garantizando el funcionamiento correcto de los libros de órdenes.

- **Isaac**:

En este proyecto aprendí de forma práctica cómo funciona un limit order book con prioridad precio–tiempo. Implementamos el matching con `std::priority_queue` y comparadores personalizados, lo que nos obligó a elegir y entender bien las estructuras de datos necesarias y a manejar fills parciales sin romper el orden temporal. También construimos un dashboard en la terminal con colores para visualizar en tiempo real bids y asks, algo que no había hecho antes. Me llevo la satisfacción de haber simulado, con código simple y claro, el funcionamiento de un CLOB real.

