-- ACCESS = access content
SELECT 
  DATE_FORMAT(NOW(),'%d %b %Y %h:%i %p') AS FECHA_CONSULTA, 
  usu.uid AS ID_USUARIO, 
  usu.name AS USUARIO
FROM users usu
WHERE usu.uid = :current_user