<?php
// Konfigurasi database
$host = "localhost";
$user = "root";      
$password = "";     
$dbname = "sensor_data";

// Membuat koneksi ke database
$conn = new mysqli($host, $user, $password, $dbname);

// Periksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}

// Periksa apakah data dikirim melalui POST
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    // Ambil data dari body request
    $angka_sumbu = isset($_POST['angka_sumbu']) ? floatval($_POST['angka_sumbu']) : null;
    $arah = isset($_POST['arah']) ? $_POST['arah'] : null;

    // Validasi data
    $valid_arah = ['kanan', 'kiri', 'depan', 'belakang'];
    if ($angka_sumbu !== null && in_array($arah, $valid_arah)) {
        // Query untuk menyimpan data
        $stmt = $conn->prepare("INSERT INTO sensor_gerak (angka_sumbu, arah) VALUES (?, ?)");
        $stmt->bind_param("ds", $angka_sumbu, $arah);

        if ($stmt->execute()) {
            echo json_encode(["status" => "success", "message" => "Data berhasil disimpan"]);
        } else {
            echo json_encode(["status" => "error", "message" => "Gagal menyimpan data"]);
        }

        $stmt->close();
    } else {
        echo json_encode(["status" => "error", "message" => "Data tidak valid"]);
    }
} else {
    echo json_encode(["status" => "error", "message" => "Hanya menerima metode POST"]);
}

$conn->close();
?>
