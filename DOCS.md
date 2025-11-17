# Documentación del Proyecto: Stock Exchange Matching Engine

## Introducción

Este proyecto implementa un motor de casamiento (matching engine) de un libro de órdenes continuo (CLOB, Continuous Limit Order Book) para una sola acción, operando en terminal. Un CLOB mantiene dos lados del mercado:

- Bids (compras): ofertas a distintos precios y cantidades.
- Asks (ventas): ofertas a distintos precios y cantidades.

La prioridad es por precio y, en caso de empate, por tiempo (price-time priority). El sistema da siempre preferencia al mejor precio y, si hay varias órdenes al mismo precio, ejecuta primero la que llegó más temprano. El proyecto ofrece:

- Inserción manual de órdenes (buy/sell) por menú.
- Emparejamiento automático al insertar una orden nueva.
- Una vista tipo “dashboard” en tiempo real en la terminal que muestra el depth (top N niveles) de bids y asks en colores.

## Investigación sobre Limit Order Books

Un Limit Order Book es la estructura central de muchos mercados electrónicos. Conceptos clave:

- Orden límite: instrucción para comprar/vender una cantidad a un precio límite (o mejor).
- Mejor bid: mayor precio de compra disponible en el libro.
- Mejor ask: menor precio de venta disponible en el libro.
- Spread: diferencia entre mejor ask y mejor bid.
- Prioridad precio-tiempo: primero mejor precio; a igual precio, se ejecuta la orden con timestamp más antiguo.

En la práctica, los libros se actualizan continuamente, mostrando profundidad por niveles de precio. El artículo “Island’s limit order book” (referido en el README) es un buen ejemplo de la interfaz esperada, trasladada aquí a un entorno de terminal.

## Diseño y Arquitectura

### Modelo de datos

Estructura `Order` (ver `Order.h`):

- price (double): precio límite de la orden.
- shares (int): cantidad de acciones.
- seq (int): secuencia incremental global para romper empates por tiempo.
- counter (static int): contador global que asigna `seq` al construir una orden.

Suposiciones sencillas: los inputs son válidos (cantidades positivas, precio positivo). Se puede extender con validaciones si es necesario.

### Estructuras de datos

Se usan `std::priority_queue<Order, vector<Order>, Comparator>` para ambos lados:

- Cola de compras (`buy_orders`) con `BuyComparator`:
	- Ordena por mayor precio primero; a igualdad de precio, menor `seq` primero (más antiguo).
- Cola de ventas (`sell_orders`) con `SellComparator`:
	- Ordena por menor precio primero; a igualdad de precio, menor `seq` primero (más antiguo).

Esto implementa la prioridad precio-tiempo de forma eficiente para siempre acceder al “top-of-book”.

### Contrato de matching

- Condición de cruce: existe trade si `bestBuy.price >= bestSell.price`.
- Cantidad ejecutada: `min(buy.shares, sell.shares)`.
- Precio de ejecución: se fija al lado pasivo (aquí usamos `sell.price`).
- Órdenes residuales: si sobra cantidad en buy o sell, se reinsertan con la cantidad restante conservando su `seq` original.

Diagrama de flujo (simplificado):

```
Nueva orden -> addBuy/addSell -> match()
	while bestBuy && bestSell:
		if bestBuy.price < bestSell.price: parar
		traded = min(bestBuy.qty, bestSell.qty)
		ejecutar(traded, precio = bestSell.price)
		actualizar remanentes
		reinsertar si qty > 0
```

Complejidad aproximada:

- Inserción en cola de prioridad: O(log N)
- Consulta del mejor nivel: O(1)
- Matcheo: pops/reinserciones O(log N) por cada orden afectada.

### Interfaz en tiempo real (terminal)

La función `printBook()` (ver `LimitOrderBook.cpp`) genera una tabla con hasta 10 niveles por lado usando copias temporales de las colas para no mutar el estado. Se emplean códigos ANSI para colores:

- Bids en verde (mejor precio arriba).
- Asks en rojo (mejor precio arriba).
- Encabezados en cian.

El `main.cpp` limpia la pantalla en cada ciclo y dibuja el libro antes de solicitar el siguiente comando, logrando un efecto de dashboard con entrada manual.

### Decisiones y limitaciones

- Simplicidad vs. fidelidad: se prioriza un diseño claro con `priority_queue` y comparadores personalizados.
- Un solo símbolo: el README menciona “dos colas por acción”. Aquí manejamos una acción; puede extenderse con `unordered_map<string, LimitOrderBook>`.
- Entrada manual: por requerimiento del usuario, no se generan órdenes automáticas; sin embargo, la UI se refresca y permite observar el libro tras cada operación.

## Implementación

### Clases y funciones clave

- `class Order` (`Order.h` / `Order.cpp`):
	- Constructor asigna `price`, `shares` y `seq = counter++`.
	- `static int counter` inicializado en 0 al inicio de la ejecución.

- Comparadores (`Order.h`):
	- `BuyComparator`: mayor `price` primero; a igualdad, menor `seq`.
	- `SellComparator`: menor `price` primero; a igualdad, menor `seq`.

- `class LimitOrderBook` (`LimitOrderBook.h` / `.cpp`):
	- `addBuy(price, qty)` y `addSell(price, qty)`: insertan y llaman a `match()`.
	- `match()`: ejecuta mientras haya cruce; maneja fills parciales y reinserta remanentes; imprime los trades en formato `TRADE: <qty> @ <price> (Buy seq X, Sell seq Y)`.
	- `printBook() const`: muestra los top 10 niveles por lado, con columnas `Price | Qty | Seq` y colores ANSI.

- `main.cpp`:
	- Bucle principal: limpia pantalla, dibuja el libro, muestra menú y lee comandos.
	- Comandos: `b` (buy), `s` (sell), `p` (snapshot), `q` (salir).

### Fragmentos representativos (snippets)

Comparadores (concepto):

```
BuyComparator:  if (a.price != b.price) return a.price < b.price; // mayor primero
								else return a.seq > b.seq; // más antiguo primero

SellComparator: if (a.price != b.price) return a.price > b.price; // menor primero
								else return a.seq > b.seq; // más antiguo primero
```

Lógica de trade (concepto):

```
while bestBuy && bestSell:
	if bestBuy.price < bestSell.price: break
	traded = min(bestBuy.qty, bestSell.qty)
	tradePrice = bestSell.price
	imprimir(TRADE, traded, tradePrice)
	actualizar cantidades y reinsertar si queda remanente
```

## Uso y comandos

Compilación (también en README):

```
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Order.cpp LimitOrderBook.cpp -o orderbook
```

Ejecución:

```
./orderbook
```

Comandos dentro del programa:

- `b`: agregar orden de compra (se solicita precio y cantidad).
- `s`: agregar orden de venta (se solicita precio y cantidad).
- `p`: imprimir snapshot estático y pausar hasta Enter.
- `q`: salir.

## Pruebas manuales sugeridas

1) Insertar una compra que NO cruce:
	 - Buy: 100.00 x 10
	 - Sell: 101.00 x 5
	 - Resultado: no hay trade; bestBid=100.00, bestAsk=101.00.

2) Insertar una venta que SÍ cruce parcial:
	 - Ventas: 99.50 x 3 frente a Buy 100.00 x 10
	 - Resultado: TRADE de 3 @ 99.50; queda Buy 100.00 x 7.

3) Empate por precio (mismo nivel) y prioridad por tiempo:
	 - Buy A: 100.00 x 5 (seq menor)
	 - Buy B: 100.00 x 5 (seq mayor)
	 - Sell: 99.00 x 8
	 - Resultado: se ejecuta primero Buy A por 5 y luego Buy B por 3.

## Conclusiones y Aprendizajes

- Ricardo:


- Isaac:
	- 

## Trabajo futuro

- Multi-símbolo: gestionar varios libros (uno por ticker) con un `unordered_map`.
- Validaciones de entrada y manejo de errores (precios/cantidades inválidos).
- Entrada no bloqueante y/o mejor interacción en tiempo real (por ejemplo, `ncurses`).
- Persistencia o logging de trades para análisis posterior.