
<?php
$this['video'] = 'active';

if($_POST){
	// validation of data
	$fields = array(
	    'id_video' => array('required' => true, 'filter' => FILTER_VALIDATE_INT),
	);

	if (($data = $this->filter($_POST, $fields)) === false) {
	    $this->flash($this['app.filters.messages'], 'error');
	}else{

		// remove video
		$this['db']->delete('videos', array('id' => $data['id_video']));
		
		$this->flash('Videos successfully removed from the list!', 'success');
	}
}

$videos = $this['db']->select('videos');

