<?php

if($_POST){
	Atomik::disableLayout();
	$fields = array(
	    'name' => array('required' => true),
	    'mac' => array('required' => true),
	    'disk_available' => array('required' => true)
	);

	if (($data = $this->filter($_POST, $fields)) === false) {
	    $this->flash($this['app.filters.messages'], 'error');
	    var_dump($_POST);
	    echo "error A";
	    return;
	}

	// INSERT or UPDATE
	$stmt = $this['db']->prepare('INSERT INTO devices (id, ip, name, mac, disk_available, last_login) VALUES (NULL, :ip, :name, :mac, :disk_available, NOW()) ON DUPLICATE KEY UPDATE ip=:ip, name = :name, disk_available = :disk_available, last_login = NOW();');
	$stmt->execute(array(
		'ip'				=> $_SERVER['REMOTE_ADDR'],
		'name'				=> $data['name'],
		'mac'				=> $data['mac'],
		'disk_available'	=> $data['disk_available'],
	));

	$id = $this['db']->lastInsertId ("id");

	$device = $this['db']->selectOne('devices', array('mac' => $data['mac']));

	$scene = $this['db']->selectOne('scenes', array('id' => $device['id_scene']));


	$this['db']->update('devices', array('wipeout_cache' => 0), array('id' => $id));

	if(!$scene){
		$result = array(
			'url' 	=> '',
			'id_video' => -1,
			'video' => '',
			'x' => '0',
			'y' => '0',
			'width' => '0',
			'height' => '0',
			'version' => -1,
			'id' => -1,
			'wipeout_cache' => $device['wipeout_cache']
			);
	}else{
		$result = $scene;
		$video = $this['db']->selectOne('videos', array('id' => $scene['id_video']));
		$result['video'] = $video['url'];
		$result['wipeout_cache'] = $device['wipeout_cache'];
	}

	
	echo json_encode($result);
	
}else{
	echo "No POST data received.";
}