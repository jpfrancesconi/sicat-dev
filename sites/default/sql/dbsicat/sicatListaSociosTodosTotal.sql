-- ACCESS = access content
SELECT 
	COUNT(*) AS TOTAL_SOCIOS
FROM `io_socio` soc 
JOIN io_persona per ON per.id_persona = soc.persona_id
WHERE 1=1
--IF=:apellido
AND per.apellidos like CONCAT('%',:apellido,'%')
--END