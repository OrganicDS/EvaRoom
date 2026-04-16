<?php
// ---- CONFIG ---- (idéntico a eva-datos.php)
$API_KEY_SERVER = 'tPmAT5Ab3j7F9';
$DB_HOST = 'localhost';
$DB_NAME = 'c1371778_eva0123';
$DB_USER = 'c1371778_eva0123';
$DB_PASS = 'zoWA08pinu';
$TABLE   = 'sensor_data';

$apikey = $_GET['apikey'] ?? '';
$nserie = $_GET['nserie'] ?? '';
$format = strtolower($_GET['format'] ?? 'csv'); // csv | xls
$ppm_factor = isset($_GET['ppm']) && (float)$_GET['ppm'] == 0.5 ? 0.5 : 0.7;

if ($apikey !== $API_KEY_SERVER || $nserie === '') {
  http_response_code(403);
  echo "Acceso denegado.";
  exit;
}

$dsn = "mysql:host=$DB_HOST;dbname=$DB_NAME;charset=utf8mb4";
$pdo = new PDO($dsn, $DB_USER, $DB_PASS, [
  PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
  PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
]);

$st = $pdo->prepare("SELECT created_at, humSustrato, tempSustrato, conductivity, pH, nitrogen, phosphorus, potassium
                     FROM $TABLE 
                     WHERE serie = ? AND created_at >= DATE_SUB(NOW(), INTERVAL 3 MONTH)
                     ORDER BY created_at DESC");
$st->execute([$nserie]);
$rows = $st->fetchAll();

$factor = $ppm_factor==0.5 ? 500 : 700;

if ($format === 'json') { // por si más adelante lo querés
  header('Content-Type: application/json; charset=utf-8');
  echo json_encode($rows, JSON_UNESCAPED_UNICODE|JSON_UNESCAPED_SLASHES);
  exit;
}

if ($format === 'xls') {
  header('Content-Type: application/vnd.ms-excel; charset=utf-8');
  header('Content-Disposition: attachment; filename="eva_7valores_3m_serie_'.$nserie.'.xls"');
  echo "<table border='1'>";
  echo "<tr><th>fecha</th><th>humidity_%</th><th>temp_C</th><th>ec_uS_cm</th><th>ec_mS_cm</th><th>tds_ppm</th><th>pH</th><th>N_mgkg</th><th>P_mgkg</th><th>K_mgkg</th></tr>";
  foreach ($rows as $r) {
    $ec_uS = (float)$r['conductivity'];
    $ec_mS = $ec_uS / 1000.0;
    $ppm   = $ec_mS * $factor;
    echo "<tr>";
    echo "<td>".htmlspecialchars($r['created_at'])."</td>";
    echo "<td>".number_format((float)$r['humSustrato'],1,'.','')."</td>";
    echo "<td>".number_format((float)$r['tempSustrato'],1,'.','')."</td>";
    echo "<td>".number_format($ec_uS,0,'.','')."</td>";
    echo "<td>".number_format($ec_mS,3,'.','')."</td>";
    echo "<td>".number_format($ppm,0,'.','')."</td>";
    echo "<td>".number_format((float)$r['pH'],1,'.','')."</td>";
    echo "<td>".number_format((float)$r['nitrogen'],0,'.','')."</td>";
    echo "<td>".number_format((float)$r['phosphorus'],0,'.','')."</td>";
    echo "<td>".number_format((float)$r['potassium'],0,'.','')."</td>";
    echo "</tr>";
  }
  echo "</table>";
  exit;
}

// CSV por defecto
header('Content-Type: text/csv; charset=utf-8');
header('Content-Disposition: attachment; filename="eva_7valores_3m_serie_'.$nserie.'.csv"');
$out = fopen('php://output', 'w');
fputcsv($out, ['fecha','humidity_%','temp_C','ec_uS_cm','ec_mS_cm','tds_ppm','pH','N_mgkg','P_mgkg','K_mgkg']);
foreach ($rows as $r) {
  $ec_uS = (float)$r['conductivity'];
  $ec_mS = $ec_uS / 1000.0;
  $ppm   = $ec_mS * $factor;
  fputcsv($out, [
    $r['created_at'],
    number_format((float)$r['humSustrato'],1,'.',''),
    number_format((float)$r['tempSustrato'],1,'.',''),
    number_format($ec_uS,0,'.',''),
    number_format($ec_mS,3,'.',''),
    number_format($ppm,0,'.',''),
    number_format((float)$r['pH'],1,'.',''),
    number_format((float)$r['nitrogen'],0,'.',''),
    number_format((float)$r['phosphorus'],0,'.',''),
    number_format((float)$r['potassium'],0,'.',''),
  ]);
}
fclose($out);
