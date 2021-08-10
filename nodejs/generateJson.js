
// import UrlParse from 'url-parse';
// const urlParser = new UrlParse(window.location.href, true);

let fs = require('fs')
let AdmZip = require('adm-zip');
let shellJs = require('shelljs');

const fileDir = '/Users/bryan.wu/Downloads/banuba-upgrade/json';

fs.readdir(fileDir, function (err, files) {
	if (err) {
		return console.log('目录不存在');
	}
	//console.log(files);

	let resource = {};
	resource['effect'] = [];

	files.forEach((fileName) => {
		if (fileName.indexOf('.zip') == -1
		&& fileName.indexOf('.json') == -1
		) {
			console.log(fileName);
			let item = {}
			const itemName = fileName.substr(0, fileName.indexOf('-'));
			const itemThumbnailUrl = 'https://app.dev.glip.net/assets/mobile/media/banuba/effect/' + fileName;
			let itemVersion  = fileName.substr(itemName.length + 1);
			itemVersion = itemVersion.replace(".png", "");
			item['name'] = itemName;
			item['thumbnail_url'] = itemThumbnailUrl;
			item['remote_dir'] = 'mobile/media/banuba/effect';
			item['local_dir'] = 'banuba/effect';
			item['version'] = itemVersion;
			item['file_format'] = '.zip';
			resource['effect'].push(item);
		}


		// const zipFile = fileDir + '/' + fileName;
		// const name = fileName.substr(0, fileName.lastIndexOf('.'));
		// const extractDir = fileDir + '/' + name;
		// let zip = new AdmZip(zipFile);
		// zip.extractAllTo(extractDir, /*overwrite*/true);

		// const thumbanilName = extractDir + '/' + name + '.png';
		// const prewName = extractDir + '/preview.png';
		// const targetDir = '/Users/bryan.wu/niceice/service/frontend/files/files/';
		// fs.rename(prewName, thumbanilName, (error) => {
		// 	if (error) {
		// 	  console.log(error);
		// 	} else {
		// 	  //console.log('ok');
		// 	}
		//       });

		// shellJs.cp('-f', thumbanilName, targetDir)
	});


	
	const resourceJson = JSON.stringify(resource, null, 2);
	fs.writeFileSync('resource.json', resourceJson);
	shellJs.cp('-f', './resource.json', fileDir);
	const niceFileDir = '/Users/bryan.wu/niceice/service/frontend/files/files';
	const pubFileDir = '/Users/bryan.wu/work_studio/public-assets/src/default/develop/common/media/banuba/effect';
	shellJs.cp('-f', './resource.json', fileDir);
	shellJs.cp('-f', './resource.json', niceFileDir);
	shellJs.cp('-f', './resource.json', pubFileDir);
	console.log(resourceJson);

});

