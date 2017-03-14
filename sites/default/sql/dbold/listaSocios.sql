-- ACCESS = access content
SELECT 
	Soc_NroSocio AS NRO_SOCIO,
	Soc_Apellido AS APELLIDO,
	Soc_DomicilioPart AS DOMICILIO,
	Soc_Telefono AS TELEFONO
FROM `old_Socios` 
WHERE 1=1
--SWITCH=:soc_cod_baja
--CASE=ACTIVO
AND Soc_Cod_Baja = 0
--CASE=NO ACTIVO
AND Soc_Cod_Baja != 0
--CASE=TODOS
AND 1=1
--END
--IF=:apellido
AND Soc_Apellido like CONCAT('%',:apellido,'%')
--END