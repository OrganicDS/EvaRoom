<?php
// ---- CONFIG ----
$API_KEY_SERVER = 'tPmAT5Ab3j7F9'; // ponelo igual que apiKeyValue del ESP32 (o mejor: muévelo a config privada)
$DB_HOST = 'localhost';
$DB_NAME = 'c1371778_eva0123';
$DB_USER = 'c1371778_eva0123';
$DB_PASS = 'zoWA08pinu';
$TABLE   = 'sensor_data';

// ---- AUTENTICACIÓN SIMPLE ----
$apikey = $_GET['apikey'] ?? '';
$nserie = $_GET['nserie'] ?? '';
if ($apikey !== $API_KEY_SERVER || $nserie === '') {
  http_response_code(403);
  echo "Acceso denegado.";
  exit;
}

// ---- CONEXIÓN ----
$dsn = "mysql:host=$DB_HOST;dbname=$DB_NAME;charset=utf8mb4";
$pdo = new PDO($dsn, $DB_USER, $DB_PASS, [
  PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
  PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
]);

// ---- PARÁMETROS ----
$ppm_factor = isset($_GET['ppm']) && (float)$_GET['ppm'] == 0.5 ? 0.5 : 0.7;
$page = max(1, (int)($_GET['page'] ?? 1));
$perPage = 200;
$offset = ($page-1) * $perPage;

// ---- TOTAL y DATOS (últimos 3 meses) ----
$st = $pdo->prepare("SELECT COUNT(*) AS c FROM $TABLE 
                     WHERE serie = ? AND created_at >= DATE_SUB(NOW(), INTERVAL 3 MONTH)");
$st->execute([$nserie]);
$total = (int)$st->fetchColumn();

$st = $pdo->prepare("SELECT created_at, humSustrato, tempSustrato, conductivity, pH, nitrogen, phosphorus, potassium
                     FROM $TABLE 
                     WHERE serie = ? AND created_at >= DATE_SUB(NOW(), INTERVAL 3 MONTH)
                     ORDER BY created_at DESC
                     LIMIT $perPage OFFSET $offset");
$st->execute([$nserie]);
$rows = $st->fetchAll();
$totalPages = max(1, (int)ceil($total / $perPage));

function h($s){ return htmlspecialchars((string)$s, ENT_QUOTES, 'UTF-8'); }
?>
<!doctype html>
<html lang="es">
<head>
  <meta charset="utf-8">
  <title>Datos – últimos 3 meses (serie <?=h($nserie)?>)</title>
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <style>
    body{font-family:system-ui,-apple-system,Segoe UI,Roboto,Ubuntu,Arial,sans-serif;background:#0b0e14;color:#eaeef3;margin:0;padding:24px}
    .wrap{max-width:1200px;margin:0 auto}
    a.btn{padding:8px 12px;border:1px solid #2c3a4a;border-radius:10px;text-decoration:none;color:#eaeef3;background:#131a24;display:inline-block}
    a.btn:hover{background:#182131}
    .toolbar{display:flex;gap:8px;flex-wrap:wrap;margin:0 0 16px}
    table{width:100%;border-collapse:collapse;background:#0f1420}
    th,td{border-bottom:1px solid #223046;padding:8px 10px;font-size:14px;text-align:right}
    th:first-child,td:first-child{text-align:left;white-space:nowrap}
    thead th{position:sticky;top:0;background:#0f1420}
    .muted{opacity:.75;margin:8px 0 12px}
    .pager{margin:14px 0;display:flex;gap:6px;flex-wrap:wrap}
  </style>
</head>
<body>
  <div class="wrap">
    <h1>Serie <?=h($nserie)?> — últimos 3 meses</h1>

    <div class="toolbar">
      <a class="btn" href="?apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=0.5">PPM ×0.5</a>
      <a class="btn" href="?apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=0.7">PPM ×0.7</a>
    </div>

    <div class="muted">Registros: <?=h($total)?> • Página <?=h($page)?>/<?=h($totalPages)?> • PPM factor: <?=h($ppm_factor)?></div>

    <table>
      <thead>
        <tr>
          <th>Fecha</th>
          <th>Hum. (%)</th>
          <th>Temp (°C)</th>
          <th>EC (µS/cm)</th>
          <th>EC (mS/cm)</th>
          <th>TDS (ppm)</th>
          <th>pH</th>
          <th>N (mg/kg)</th>
          <th>P (mg/kg)</th>
          <th>K (mg/kg)</th>
        </tr>
      </thead>
      <tbody>
        <?php
        $factor = $ppm_factor==0.5 ? 500 : 700;
        foreach ($rows as $r):
          $ec_uS = (float)($r['conductivity'] ?? 0);
          $ec_mS = $ec_uS / 1000.0;
          $ppm   = $ec_mS * $factor;
        ?>
        <tr>
          <td><?=h($r['created_at'])?></td>
          <td><?=number_format((float)$r['humSustrato'], 1, ',', '.')?></td>
          <td><?=number_format((float)$r['tempSustrato'], 1, ',', '.')?></td>
          <td><?=number_format($ec_uS, 0, ',', '.')?></td>
          <td><?=number_format($ec_mS, 2, ',', '.')?></td>
          <td><?=number_format($ppm, 0, ',', '.')?></td>
          <td><?=number_format((float)$r['pH'], 1, ',', '.')?></td>
          <td><?=number_format((float)$r['nitrogen'], 0, ',', '.')?></td>
          <td><?=number_format((float)$r['phosphorus'], 0, ',', '.')?></td>
          <td><?=number_format((float)$r['potassium'], 0, ',', '.')?></td>
        </tr>
        <?php endforeach; ?>
      </tbody>
    </table>

    <div class="pager">
      <?php if ($page>1): ?>
        <a class="btn" href="?apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=<?=h($ppm_factor)?>&page=1">« Primero</a>
        <a class="btn" href="?apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=<?=h($ppm_factor)?>&page=<?=h($page-1)?>">‹ Anterior</a>
      <?php endif; ?>
      <?php if ($page<$totalPages): ?>
        <a class="btn" href="?apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=<?=h($ppm_factor)?>&page=<?=h($page+1)?>">Siguiente ›</a>
        <a class="btn" href="?apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=<?=h($ppm_factor)?>&page=<?=h($totalPages)?>">Último »</a>
      <?php endif; ?>
    </div>

    <div class="toolbar" style="margin-top:14px">
      <a class="btn" href="eva-datos-export.php?format=csv&apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=<?=h($ppm_factor)?>">⬇️ Descargar CSV (3 meses)</a>
      <a class="btn" href="eva-datos-export.php?format=xls&apikey=<?=h($apikey)?>&nserie=<?=h($nserie)?>&ppm=<?=h($ppm_factor)?>">⬇️ Descargar XLS (3 meses)</a>
    </div>
  </div>
</body>
</html>
