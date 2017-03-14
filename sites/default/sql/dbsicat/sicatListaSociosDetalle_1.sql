-- ACCESS = access content
SELECT soc.`secuencia` AS SECUENCIA,
 	per.apellidos AS APELLIDO,
    gfs.`parentezco` AS PARENTESCO
 FROM `io_grupo_familiar_socio` gfs
 JOIN `io_socio` soc ON soc.`id_socio` = gfs.`socio_id`
 JOIN io_persona per ON per.`id_persona` = soc.`persona_id`
 JOIN `io_grupo_familiar` gf ON gfs.`grupo_familiar_id` = gf.`id_grupo_familiar`
 WHERE gf.`nro_socio_titular` = :NRO_SOCIO
 AND soc.`secuencia` != 0