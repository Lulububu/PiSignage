<?php

$this['scene'] = 'active';

$id = -1;

// In case of an edit
if($_POST){
	// validation of data
	$fields = array(
	    'name' => array('required' => true),
	    'description' => array('required' => true),
	    'url' => array('required' => true, 'filter' => FILTER_VALIDATE_URL),
	    'id_video' => array('required' => false, 'filter' => FILTER_VALIDATE_INT),
	    'x' => array('required' => false, 'filter' => FILTER_VALIDATE_INT),
	    'y' => array('required' => false, 'filter' => FILTER_VALIDATE_INT),
	    'width' => array('required' => false, 'filter' => FILTER_VALIDATE_INT),
	    'height' => array('required' => false, 'filter' => FILTER_VALIDATE_INT),
	    'id' => array('required' => true, 'filter' => FILTER_VALIDATE_INT)
	);

	if (($data = $this->filter($_POST, $fields)) === false) {
	    $this->flash("Every fields are required. ".$this['app.filters.messages'], 'error');
	    
	}else{
		// if it is a new scene
		if($data['id'] == -1){
			$data['id'] = NULL;
			$this['db']->insert('scenes', $data);
			$id = $this['db']->lastInsertId ("id");
		}else{
			// update of a scene
			$this['db']->update('scenes', $data, array('id' => $data['id']));
			// update the version
			$stmt = $this['db']->prepare('UPDATE `scenes` SET `version`=`version` + 1 WHERE id=:id');
			$stmt->execute(array(
				'id' => $data['id']
				));
			$id = $data['id'];
		}
		
		$this->flash('Scene successfully edited!', 'success');
	}
}else{
	$id = Atomik::get('request.id');
}



if($id != -1){
	$scene = $this['db']->selectOne('scenes', array('id' => $id));
}else{
	$scene = array(
		'name' 			=> '',
		'description' 	=> '',
		'url' 			=> '',
		'video' 		=> '',
		'id_video'		=> -1,
		'x' 			=> '0',
		'y' 			=> '0',
		'width' 		=> '800',
		'height'		=> '600'
		);
}

$videos = $this['db']->select('videos');




