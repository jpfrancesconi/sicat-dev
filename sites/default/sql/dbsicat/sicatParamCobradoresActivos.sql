-- ACCESS = access content
SELECT 
	p.id_personal AS ID_PERSONAL,
	CONCAT(per.apellidos, ' ', per.nombres) AS APE_NOM_COBRADOR
FROM `io_personal` p  
JOIN io_persona per ON per.id_persona = p.persona_id
WHERE p.activo = TRUE
ORDER BY p.id_personal ASC