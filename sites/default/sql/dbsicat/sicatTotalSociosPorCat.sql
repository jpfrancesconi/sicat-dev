-- ACCESS = access content
SELECT cs.`id_categoria_socio` AS CODIGO, 
	cs.NOMBRE AS CATEGORIA, 
	COUNT(*) AS CANTIDAD
FROM io_socio soc 
JOIN `io_categoria_socio` cs ON cs.`id_categoria_socio` = soc.`categoria_socio_id`
WHERE soc.tipo_socio_id = 1
GROUP BY cs.nombre ASC
ORDER BY cs.nombre ASC;