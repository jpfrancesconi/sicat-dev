-- ACCESS = access content
SELECT adh.Adh_Secuencia AS SECUENCIA, 
    adh.Adh_Apellido AS APELLIDO,
    IF(adh.Adh_Parentesco IS NULL,'NO DEFINIDO',adh.Adh_Parentesco) AS PARENTESCO
FROM `old_Socios` soc
JOIN old_Adherentes adh ON adh.Adh_NroSocioTitular = soc.Soc_NroSocio
WHERE Soc_NroSocio like :NRO_SOCIO