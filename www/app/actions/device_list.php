<?php
$this['device'] = 'active';


if($_POST){
	// validation of data
	$fields = array(
	    'id_device' => array('required' => true, 'filter' => FILTER_VALIDATE_INT),
	    'command'	=> array()
	);

	if (($data = $this->filter($_POST, $fields)) === false) {
	    $this->flash($this['app.filters.messages'], 'error');
	}else{
		if($data['command'] == 'remove'){
			// remove device
			$this['db']->delete('devices', array('id' => $data['id_device']));
			$this->flash('Device successfully deleted!', 'success');
		}else if($data['command'] == 'wipeout_cache'){
			$update = array();
			$update['wipeout_cache'] = 1;
			$this['db']->update('devices', $update, array('id' => $data['id_device']));
		}
	}
}


$stmt = $this['db']->prepare('SELECT `devices`.* , `scenes`.`name` as `scene_name` FROM `devices` LEFT OUTER JOIN `scenes` ON `devices`.`id_scene` = `scenes`.`id`;');

$stmt->execute();
$devices = $stmt->fetchAll();


function formatBytes($size, $precision = 2)
{
    $base = log($size) / log(1024);
    $suffixes = array('', 'kB', 'MB', 'GB', 'TB');   

    return round(pow(1024, $base - floor($base)), $precision) . $suffixes[floor($base)];
}


function getStatus($date){
	$curr = new DateTime();
	$dateWarning = new DateTime($date);
	$dateWarning->add(new DateInterval('PT2M'));

	$dateError = new DateTime($date);
	$dateError->add(new DateInterval('PT10M'));



	if($dateError < $curr) {
		return 'error';
	}else if($dateWarning < $curr){
		return 'warning';
	}else{
		return 'success';
	}
	
}

