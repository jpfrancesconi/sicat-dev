-- ACCESS = access content
SELECT * 
FROM  `old_CtasCtes` cc1
JOIN old_Cpto_cta_cte ccc ON ccc.concepto_cta_cte = cc1.Cta_concepto
WHERE Cta_Nro_Soc LIKE  :NRO_SOCIO
AND cc1.Cta_Nro_Soc NOT IN ( SELECT cc2.Cta_Nro_Soc
	FROM old_CtasCtes cc2
	WHERE cc1.Cta_Concepto = cc2.Cta_Concepto
		AND cc1.Cta_anio = cc2.Cta_Anio
		AND cc1.Cta_Mes = cc2.Cta_Mes
		AND cc2.Cta_Concepto_CC = 99
							)
ORDER BY cc1.Cta_Anio ASC, cc1.Cta_Mes ASC