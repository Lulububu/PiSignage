<?php
$this['device'] = 'active';
// In case of an edit
if($_POST){
	// validation of data
	$fields = array(
	    'id_scene' => array('required' => true, 'filter' => FILTER_VALIDATE_INT),
	    'id' => array('required' => true, 'filter' => FILTER_VALIDATE_INT)
	);

	if (($data = $this->filter($_POST, $fields)) === false) {
	    $this->flash($this['app.filters.messages'], 'error');
	}else{
		// update of a device
		$this['db']->update('devices', $data, array('id' => $data['id']));
		$id = $data['id'];
		$this->flash('Deice successfully edited!', 'success');
	}

	
	
}else{
	$id = Atomik::get('request.id');
}



if($id != -1){

	$device = $this['db']->selectOne('devices', array('id' => $id));
}else{
	$device = array(
		'ip' 			=> '',
		'name' 	=> '',
		'mac' 			=> '',
		'id_scene' 			=> ''
		);
}

$scenes = $this['db']->select('scenes');

