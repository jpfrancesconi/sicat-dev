-- ACCESS = access content
SELECT tgf.`id_tipo_grupo_familiar` AS CODIGO, 
	tgf.NOMBRE AS GRUPO, 
	COUNT(*) AS CANTIDAD
FROM io_socio soc 
JOIN `io_grupo_familiar_socio` gfs ON gfs.`socio_id` = soc.`id_socio`
JOIN `io_grupo_familiar` gf ON gf.`id_grupo_familiar` = gfs.`grupo_familiar_id`
JOIN `io_tipo_grupo_familiar` tgf ON tgf.`id_tipo_grupo_familiar` = gf.`tipo_grupo_familiar_id`
WHERE soc.tipo_socio_id = 2 or soc.`tipo_socio_id` = 3
GROUP BY tgf.nombre ASC
ORDER BY tgf.`id_tipo_grupo_familiar` ASC;