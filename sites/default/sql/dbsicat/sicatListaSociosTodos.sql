-- ACCESS = access content
SELECT 
	soc.nro_socio AS NRO_SOCIO,
	soc.secuencia AS NRO_SEC,
	per.apellidos AS APELLIDO,
	per.direccion AS DOMICILIO,
	per.telefono_fijo AS TELEFONO,
	CASE soc.secuencia WHEN 0 THEN 1 ELSE 0 END AS PRINCIPAL
FROM `io_socio` soc 
JOIN io_persona per ON per.id_persona = soc.persona_id
WHERE 1=1
--IF=:apellido
AND per.apellidos like CONCAT('%',:apellido,'%')
--END
ORDER BY soc.nro_socio ASC, soc.secuencia ASC