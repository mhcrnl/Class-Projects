/* GroupID: 16 */
/* SID1: 84698109, NAME: Christian Morte */
/* SID2: 15696987, NAME: Raymond Lam */
/* SID3: 34818760, NAME: Lisa Tran */

USE cs122a;

/* Q1 */
SELECT l.airport_IATA_code, COUNT(l.airport_IATA_code)
FROM Lounge l
GROUP BY l.airport_IATA_code;

/* Q2 */
SELECT f.flight_number, TIMESTAMPDIFF(SECOND, f.actual_departure_datetime, f.actual_arrival_datetime) AS time_diff
FROM Flight f
WHERE f.projected_departure_datetime = ( SELECT MAX(f2.projected_departure_datetime)
										FROM Flight f2
										);

/* Q3 */
SELECT MAX(ABS( TIMESTAMPDIFF(SECOND, f.projected_departure_datetime, f.projected_arrival_datetime) - TIMESTAMPDIFF(SECOND, f.actual_departure_datetime, f.actual_arrival_datetime) )) AS max_diff
FROM Flight f;

/* Q4 */
SELECT DISTINCT f.flight_number, 
MAX(ABS( TIMESTAMPDIFF(SECOND, f.projected_departure_datetime, f.projected_arrival_datetime) - TIMESTAMPDIFF(SECOND, f.actual_departure_datetime, f.actual_arrival_datetime) )) AS max_diff, 
MIN(ABS( TIMESTAMPDIFF(SECOND, f.projected_departure_datetime, f.projected_arrival_datetime) - TIMESTAMPDIFF(SECOND, f.actual_departure_datetime, f.actual_arrival_datetime) )) AS min_diff
FROM Flight f
GROUP BY f.flight_number;

/* Q5 */
-- SELECT pf.pid, pf.num_of_flights
-- FROM (SELECT p.pid, COUNT(*) AS num_of_flights
-- 	  FROM Pilot_Operates_Flight p
-- 	  GROUP BY p.pid) as pf
-- WHERE pf.num_of_flights = (SELECT MAX(pf.num_of_flights)
-- 						   FROM (SELECT p.pid, COUNT(*) AS num_of_flights
-- 								 FROM Pilot_Operates_Flight p
-- 								 GROUP BY p.pid) as pf);


SELECT p.pid, COUNT(p.projected_departure_datetime) AS num_of_flights
FROM Pilot_Operates_Flight p
GROUP BY p.pid
HAVING num_of_flights = (SELECT MAX(pf.num_of_flights)
						 FROM (  SELECT p.pid, COUNT(*) AS num_of_flights
								 FROM Pilot_Operates_Flight p
								 GROUP BY p.pid) AS pf);


/* Q6 */
-- SELECT avg_menu_prices.lid, avg_menu_prices.avg_price
-- FROM (SELECT d.lid, AVG(d.price) AS avg_price
-- 	  FROM Dish d
-- 	  GROUP BY d.lid) AS avg_menu_prices
-- WHERE avg_menu_prices.avg_price > (SELECT AVG(avg_price) AS avg_overall_price
-- 								   FROM (SELECT d.lid, AVG(d.price) AS avg_price
-- 										 FROM Dish d
-- 										 GROUP BY d.lid) AS menu_prices);
                                         
SELECT d.lid, AVG(d.price) as avg_price
FROM Dish d
GROUP BY d.lid
HAVING avg_price > (SELECT AVG(d2.price)
					FROM Dish d2);
                                         
/* Q7 */
SELECT d.oid, d.name, d.quantity
FROM DishOrder_Contains_Dish d, (SELECT d.oid
								 FROM DishOrder_Contains_Dish d
								 GROUP BY d.oid
								 HAVING COUNT(d.oid) >= 2) AS doids
WHERE d.oid = doids.oid;


/* Q8 */
SELECT temp.cid, SUM(temp.total_price)
FROM (SELECT c.cid, SUM(cf.purchased_price) AS total_price
	  FROM Customer c LEFT OUTER JOIN Customer_Reserves_Flight cf ON c.cid = cf.cid
	  GROUP BY c.cid, cf.purchased_price
	  ORDER BY c.cid) AS temp
GROUP BY temp.cid
ORDER BY temp.cid;


/* Q9 */
SELECT l.airport_IATA_code, COUNT(d.oid)
FROM Lounge l LEFT JOIN DishOrder d ON l.lid = d.lid
GROUP BY l.airport_IATA_code
ORDER BY l.airport_IATA_code;

/* Q10 */
SELECT l.lid, l.airport_IATA_code, COUNT(d.lid) AS num_of_dishes, 
				MIN(d.price) AS lowest_price, MAX(d.price) AS highest_price, AVG(d.price) AS avg_price
FROM Lounge l LEFT JOIN Dish d ON l.lid = d.lid
GROUP BY l.lid
ORDER BY l.lid



                                         
